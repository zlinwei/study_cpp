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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        LOG(INFO) << "Usage :" << argv[0] << " [path]";
        return -1;
    }
    nn::Sequential resnet18(
            nn::Conv2d(nn::Conv2dOptions(1, 64, {7, 7}).stride(2).padding(1).with_bias(false)),
            nn::Functional(torch::leaky_relu, 0.2)
    );

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
            resnet18->parameters(), torch::optim::AdamOptions(2e-4).beta1(0.5));
    torch::optim::Adam discriminator_optimizer(
            resnet18->parameters(), torch::optim::AdamOptions(5e-4).beta1(0.5));

    for (int64_t epoch = 1; epoch <= 10; ++epoch) {
        int64_t batch_index = 0;
        for (auto &batch : *data_loader) {
            // Train discriminator with real images.
            resnet18->zero_grad();
//            auto real_images = batch.data;
//            torch::Tensor real_labels = torch::empty(batch.data.size(0)).uniform_(0.8, 1.0);
//            torch::Tensor real_output = resnet18->forward(real_images);
//            torch::Tensor d_loss_real = torch::binary_cross_entropy(real_output, real_labels);
//            d_loss_real.backward();

            LOG(INFO) << batch_index++;
        }
    }

    return 0;
}