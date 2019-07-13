//
// Created by linwei on 2019-07-13.
//

#include <iostream>
#include <torch/torch.h>
#include <glog/logging.h>

int main(int, char *[]) {
    torch::Tensor x = torch::ones({5, 3}, torch::requires_grad());
    LOG(INFO) << x;

    x = torch::randn_like(x);
    LOG(INFO) << x;

    return 0;
}