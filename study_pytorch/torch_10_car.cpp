//
// Created by Administrator on 2019/7/30.
//


#include "CarDataset.hpp"

#include <glog/logging.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        LOG(INFO) << "Usage: " << argv[0] << " [path]";
        return -1;
    }
    std::string path = argv[1];
//        auto dataset = HumanProteinAtlasDataset(path).map(torch::data::transforms::Stack<>());
    auto dataset = CarDataset(path);
    LOG(INFO) << "data size: " << *dataset.size();
    for (int i = 0; i < *dataset.size(); ++i) {
        auto item = dataset.get(i);
        std::cout << item.data.sizes() << "\n";
        std::cout << item.target << "\n";
    }
    std::cout << std::endl;
    return 0;
}