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

#include "ResNet18.hpp"



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
				torch::Tensor label;
				torch::Tensor real_output;
                for (int i = 0; i < batch_size; ++i) {
                    LOG(INFO) << "get sample: " << samp_index;
                    auto data = dataset.get(samp_index);
                    auto image = data.data.view({1, 1, 512, 512});
                    label = data.target.view({1, 1, 28});
                    real_output = resNet18->forward(image);
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