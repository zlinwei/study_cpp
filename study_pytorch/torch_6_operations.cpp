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

    std::vector<float> yv = {1, 3, 5, 7, 9, 2};
    //2 rows, 3 cols
    torch::Tensor y = torch::from_blob(yv.data(), {2, 3}, at::kFloat);
    LOG(INFO) << y;

    return 0;
}