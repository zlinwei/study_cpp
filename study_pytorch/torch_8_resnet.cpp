#include <utility>

#include <utility>

//
// Created by linwei on 2019-07-19.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>

#include <boost/format.hpp>
#include <torch/torch.h>
#include <glog/logging.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "HumanProteinDatasets.hpp"

using namespace torch;

/**
 * conv1 7x7,64,stride 2
 */


class ConvBlock2Impl : public torch::nn::Module {
public:
    ConvBlock2Impl(int input_ch, int output_ch, int kernel_size) :
            _conv1(register_module("conv1", nn::Conv2d(
                    nn::Conv2dOptions(input_ch, output_ch, {kernel_size, kernel_size}).stride(1))
            )),
            _bn1(register_module("bn1",
                                 nn::BatchNorm(output_ch)
            )),
            _conv2(register_module("conv2", nn::Conv2d(
                    nn::Conv2dOptions(output_ch, output_ch, {kernel_size, kernel_size})
                            .stride(1))
            )),
            _bn2(register_module("bn2", nn::BatchNorm(output_ch))) {

    }

    torch::Tensor forward(torch::Tensor &x) {
        x = _conv1->forward(x);
        x = _bn1->forward(x);
        x = torch::relu(x);

        x = _conv2->forward(x);
        x = _bn2->forward(x);
        x = torch::relu(x);

        return x;
    }

private:
    nn::Conv2d _conv1{nullptr};
    nn::BatchNorm _bn1{nullptr};
    nn::Conv2d _conv2{nullptr};
    nn::BatchNorm _bn2{nullptr};
};

TORCH_MODULE(ConvBlock2);

class ResNet18Impl : public torch::nn::Module {
public:
    ResNet18Impl() :
            _conv1(register_module("conv1", nn::Conv2d(nn::Conv2dOptions(1, 64, {7, 7}).stride(2)))),
            _conv2_x(register_module("conv2_x", ConvBlock2(64, 64, 3))),
            _conv3_x(register_module("conv3_x", ConvBlock2(64, 128, 3))),
            _conv4_x(register_module("conv4_x", ConvBlock2(128, 256, 3))),
            _conv5_x(register_module("conv5_x", ConvBlock2(256, 512, 3))),
            _linear(register_module("linear", nn::Linear(512, 28))) {

    }

    Tensor forward(Tensor x) {
        x = _conv1->forward(x); //top layer
        x = torch::max_pool2d(x, 3, 2); //max pool2d
        x = _conv2_x->forward(x);
        x = _conv3_x->forward(x);
        x = _conv4_x->forward(x);
        x = _conv5_x->forward(x);
        x = torch::adaptive_avg_pool2d(x, {1, 1});
        x = x.view({x.size(0), x.size(1)});
        x = _linear->forward(x);
        x = torch::softmax(x, 1);

        return x;
    }

private:
    nn::Conv2d _conv1{nullptr};
    ConvBlock2 _conv2_x{nullptr};
    ConvBlock2 _conv3_x{nullptr};
    ConvBlock2 _conv4_x{nullptr};
    ConvBlock2 _conv5_x{nullptr};
    nn::Linear _linear{nullptr};

};

TORCH_MODULE(ResNet18);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        LOG(INFO) << "Usage :" << argv[0] << " [path]";
        return -1;
    }

    try {
        ResNet18 resNet18{};
        LOG(INFO) << "\n" << resNet18;

        torch::optim::Adam optimizer(
                resNet18->parameters(), torch::optim::AdamOptions(2e-4).beta1(0.5));


        //try to load module
        try {
            torch::load(resNet18, "resNet18-checkpoint.pt");
            torch::load(optimizer, "resNet18-optimizer-checkpoint.pt");
        } catch (std::exception &e) {
            LOG(INFO) << e.what();
        } catch (...) {
            LOG(INFO) << "Uncaught error";
        }

        std::string path = argv[1];
//        auto dataset = HumanProteinAtlasDataset(path).map(torch::data::transforms::Stack<>());
        auto dataset = HumanProteinAtlasDataset(path);
        LOG(INFO) << "data size: " << *dataset.size();

        // data loader
//        auto data_loader = torch::data::make_data_loader<>(std::move(dataset),
//                                                           torch::data::DataLoaderOptions().batch_size(1).workers(4));

        LOG(INFO) << "data loader created";

        const int batch_size = 64;
        for (int64_t epoch = 1; epoch <= 10; ++epoch) {
            const size_t total_samp = dataset.size().value();
            size_t samp_index = 0;

            LOG(INFO) << "zero_grad() before a batch";
            while (true) {
                resNet18->zero_grad();
                for (int i = 0; i < batch_size; ++i) {
                    LOG(INFO) << "get sample: " << samp_index;
                    auto data = dataset.get(samp_index);
                    auto image = data.data.view({1, 1, 512, 512});
                    auto label = data.target.view({1, 1, 28});
                    torch::Tensor real_output = resNet18->forward(image);
                    torch::Tensor d_loss_real = torch::binary_cross_entropy(real_output, label);
                    d_loss_real.backward();
                    LOG(INFO) << d_loss_real;

                    samp_index++;
                    if (samp_index >= total_samp)break;
                }

                LOG(INFO) << "label: " << label;
                LOG(INFO) << "predict: " << real_output;

                LOG(INFO) << "optimizer step()";
                optimizer.step();

                LOG(INFO) << "save to file";
                torch::save(resNet18, "resNet18-checkpoint.pt");
                torch::save(optimizer, "resNet18-optimizer-checkpoint.pt");

                if (samp_index >= total_samp)break;
            }
//            for (auto &batch : *data_loader) {
//                LOG(INFO) << "Batch size: " << batch.data.sizes() << " | Labels: "
//                          << batch.target.sizes() << std::endl;
//
//                // Train resnet with real images.
//                resNet18->zero_grad();
//                auto real_images = batch.data;
//                auto real_labels = batch.target;
//                torch::Tensor real_output = resNet18->forward(real_images);
//                torch::Tensor d_loss_real = torch::binary_cross_entropy(real_output, real_labels);
//                LOG(INFO) << d_loss_real;
//                d_loss_real.backward();
//
//                LOG(INFO) << batch_index++;
//            }
        }
    }
    catch (const std::exception &e) {
        LOG(INFO) << e.what();
    }

    return 0;
}