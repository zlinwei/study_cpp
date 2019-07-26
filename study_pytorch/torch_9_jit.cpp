//
// Created by Administrator on 2019/7/26.
//

#include <iostream>
#include <torch/script.h>
#include <glog/logging.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        LOG(ERROR) << "Usage: " << argv[0] << " [path]";
    }
    auto module = torch::jit::load(argv[1]);

    return 0;
}