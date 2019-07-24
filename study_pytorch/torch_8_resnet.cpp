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
    ConvBlock2Impl(int input_ch, int kernel_size, int output_ch) :
            _conv1(nn::Conv2dOptions(input_ch, input_ch, {kernel_size, kernel_size}).stride(2)),
            _conv2(nn::Conv2dOptions(input_ch, output_ch, {kernel_size, kernel_size}).stride(2)) {

    }

    torch::Tensor forward(torch::Tensor &x) {
        x = _conv1->forward(x);
        //TODO batch normalization and relu
        x = _conv2->forward(x);
        //TODO batch normalization and relu

        return x;
    }

private:
    nn::Conv2d _conv1;
    nn::Conv2d _conv2;
};

TORCH_MODULE(ConvBlock2);

class ResNet18Impl : public torch::nn::Module {
public:
    ResNet18Impl() :
            _conv1(nn::Conv2dOptions(1, 64, {7, 7}).stride(2)),
            _conv2_x(64, 64, 3),
            _conv3_x(64, 128, 3),
            _conv4_x(128, 256, 3),
            _conv5_x(256, 512, 3),
            _linear(512, 28) {

    }

    Tensor forward(Tensor &x) {
        x = _conv1->forward(x); //top layer
        x = torch::max_pool2d(x, 3, 2); //max pool2d
        x = _conv2_x->forward(x);
        x = _conv3_x->forward(x);
        x = _conv4_x->forward(x);
        x = _conv5_x->forward(x);
        x = torch::adaptive_avg_pool2d(x, 1);
        x = _linear->forward(x);
        x = torch::sigmoid(x);

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

    ResNet18 resNet18 = ResNet18();

    std::string path = argv[1];
    auto dataset = HumanProteinAtlasDataset(path).map(torch::data::transforms::Stack<>());
    LOG(INFO) << "data size: " << *dataset.size();
    // data loader
    auto data_loader = torch::data::make_data_loader<>(std::move(dataset),
                                                       torch::data::DataLoaderOptions().batch_size(64).workers(2));


    for (torch::data::Example<> &batch : *data_loader) {
        //size 0 is batch size
        LOG(INFO) << "Batch size: " << batch.data.size(0) << " | Labels: "
                  << batch.target.size(0) << std::endl;

        LOG(INFO) << batch.data.sizes();
        LOG(INFO) << batch.target.sizes();
        std::cout << std::endl;
    }

    torch::optim::Adam generator_optimizer(
            resNet18->parameters(), torch::optim::AdamOptions(2e-4).beta1(0.5));
    torch::optim::Adam discriminator_optimizer(
            resNet18->parameters(), torch::optim::AdamOptions(5e-4).beta1(0.5));

    for (int64_t epoch = 1; epoch <= 10; ++epoch) {
        int64_t batch_index = 0;
        for (auto &batch : *data_loader) {
            // Train discriminator with real images.
            resNet18->zero_grad();
            auto real_images = batch.data;
            auto real_labels = batch.target;
            torch::Tensor real_output = resNet18->forward(real_images);
            torch::Tensor d_loss_real = torch::binary_cross_entropy(real_output, real_labels);
            d_loss_real.backward();

            LOG(INFO) << batch_index++;
        }
    }

    return 0;
}