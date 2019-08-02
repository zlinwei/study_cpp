//
// Created by Administrator on 2019/7/30.
//


#include "CarDataset.hpp"
#include "ResNet18.hpp"

#include <glog/logging.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        LOG(INFO) << "Usage: " << argv[0] << " [path]";
        return -1;
    }
    std::string path = argv[1];
//

    try {
        CarNet carNet{3, 1};//input ch 3, output 1
        LOG(INFO) << "\n" << carNet;

        torch::optim::Adam optimizer(
                carNet->parameters(), torch::optim::AdamOptions(1e-4));


        //try to load module
        try {
            torch::load(carNet, "car-checkpoint.pt");
            torch::load(optimizer, "car-optimizer-checkpoint.pt");
        } catch (std::exception &e) {
            LOG(INFO) << e.what();
        } catch (...) {
            LOG(INFO) << "Uncaught error";
        }

        auto dataset = CarDataset(path).map(torch::data::transforms::Stack<>());
//        auto dataset = CarDataset(path);
        LOG(INFO) << "data size: " << *dataset.size();

        // data loader
        auto data_loader = torch::data::make_data_loader<>(std::move(dataset),
                                                           torch::data::DataLoaderOptions()
                                                                   .batch_size(8).workers(4));

        LOG(INFO) << "data loader created";

        for (int epoch = 0; epoch < 1000; ++epoch) {
            for (auto &batch: *data_loader) {
                optimizer.zero_grad();
                torch::Tensor images = batch.data;
                torch::Tensor output = carNet->forward(images);
                torch::Tensor labels = batch.target;
                LOG(INFO) << labels;
                LOG(INFO) << output;
                torch::Tensor loss = torch::binary_cross_entropy(output, labels);
                LOG(INFO) << "loss: " << loss.item<float>();
                loss.backward();
                optimizer.step();

            }

            torch::save(carNet, "car-checkpoint.pt");
            torch::save(optimizer, "car-optimizer-checkpoint.pt");
            LOG(INFO) << "save to file";
        }

    }
    catch (const std::exception &e) {
        LOG(INFO) << e.what();
    }

    return 0;
}