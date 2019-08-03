//
// Created by linwei on 2019-08-03.
//


#include "CarDataset.hpp"

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

        //try to load module
        try {
            torch::load(carNet, "car-checkpoint.pt");
        } catch (std::exception &e) {
            LOG(INFO) << e.what();
            return -1;
        } catch (...) {
            LOG(INFO) << "Uncaught error";
            return -1;
        }

        auto dataset = CarDataset(path).map(torch::data::transforms::Stack<>());
//        auto dataset = CarDataset(path);
        LOG(INFO) << "data size: " << *dataset.size();

        // data loader
        auto data_loader = torch::data::make_data_loader<>(std::move(dataset),
                                                           torch::data::DataLoaderOptions()
                                                                   .batch_size(1).workers(4));

        LOG(INFO) << "data loader created";

        for (auto &batch: *data_loader) {
            torch::Tensor images = batch.data;
            torch::Tensor output = carNet->forward(images);
            torch::Tensor labels = batch.target;
//            output = torch::argmax(output);
            LOG(INFO) << labels.item<float>();
            LOG(INFO) << output.item<float>();
            LOG(INFO) << "\n";
        }


    }
    catch (const std::exception &e) {
        LOG(INFO) << e.what();
    }

    return 0;
}