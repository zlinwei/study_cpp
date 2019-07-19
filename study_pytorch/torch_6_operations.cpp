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

    x = torch::ones({2, 2}, torch::requires_grad());
    LOG(INFO) << x;
    auto y1 = x + 2;
    auto z = y1 * y1 * 3;
    auto out = z.mean();

    out.backward();

    LOG(INFO) << z;
    LOG(INFO) << out;
    LOG(INFO) << x.grad();
    LOG(INFO) << y1.grad();
    LOG(INFO) << z.grad();
    LOG(INFO) << out.grad();


    torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);

    return 0;
}