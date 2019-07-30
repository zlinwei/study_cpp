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
        ResNet18 resNet18{3, 2};
        LOG(INFO) << "\n" << resNet18;

        torch::optim::Adam optimizer(
                resNet18->parameters(), torch::optim::AdamOptions(2e-3).beta1(0.5));


        //try to load module
        try {
            torch::load(resNet18, "car-checkpoint.pt");
            torch::load(optimizer, "car-optimizer-checkpoint.pt");
        } catch (std::exception &e) {
            LOG(INFO) << e.what();
        } catch (...) {
            LOG(INFO) << "Uncaught error";
        }

//        auto dataset = HumanProteinAtlasDataset(path).map(torch::data::transforms::Stack<>());
        auto dataset = CarDataset(path);
        LOG(INFO) << "data size: " << *dataset.size();

        // data loader
//        auto data_loader = torch::data::make_data_loader<>(std::move(dataset),
//                                                           torch::data::DataLoaderOptions().batch_size(1).workers(4));

        LOG(INFO) << "data loader created";

        const int batch_size = 8;
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
                    LOG(INFO) << data.data.sizes();
                    LOG(INFO) << data.target.sizes();
                    auto image = data.data.view({1, 3, 512, 512});
                    label = data.target.view({1, 1, 2});
                    real_output = resNet18->forward(image);
                    torch::Tensor d_loss_real = torch::binary_cross_entropy(real_output, label);
                    d_loss_real.backward();
                    LOG(INFO) << d_loss_real;

                    LOG(INFO) << "label: \n" << label;
                    LOG(INFO) << "predict: \n" << real_output;
                    LOG(INFO) << "optimizer step()";
                    optimizer.step();
                    resNet18->zero_grad();
                    LOG(INFO) << "save to file";
                    torch::save(resNet18, "car-checkpoint.pt");
                    torch::save(optimizer, "car-optimizer-checkpoint.pt");

                    samp_index++;
                    if (samp_index >= total_samp)break;
                }


                if (samp_index >= total_samp)break;
            }

        }
    }
    catch (const std::exception &e) {
        LOG(INFO) << e.what();
    }

    return 0;
}