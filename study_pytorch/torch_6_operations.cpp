//
// Created by linwei on 2019-07-13.
//

#include <iostream>
#include <torch/torch.h>
#include <glog/logging.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

int main(int, char *[]) {
    torch::Tensor x = torch::ones({5, 3}, torch::requires_grad());
    LOG(INFO) << x;

    x = torch::randn_like(x);
    LOG(INFO) << x;

    std::vector<float> yv = {1, 3, 5, 7, 9, 2};
    //2 rows, 3 cols
    torch::Tensor y = torch::from_blob(yv.data(), {2, 3}, at::kFloat);
    LOG(INFO) << y;


    torch::Tensor a = torch::ones({2, 2}, torch::requires_grad());
    torch::Tensor b = torch::randn({2, 2});
    auto c = a + b;
    c.backward(); // a.grad() will now hold the gradient of c w.r.t. a.


    torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);

    auto label = torch::zeros(1,torch::kLong);
    label[0] = 3;
    auto one_hot = torch::zeros(5);
    auto target = one_hot.scatter_(0, label, 1);
    LOG(INFO) << target;

    return 0;
}