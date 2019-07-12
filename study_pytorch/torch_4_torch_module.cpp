//
// Created by linwei on 2019-07-12.
//

#include <iostream>
#include <torch/torch.h>

struct NetImpl : torch::nn::Module {
    NetImpl(int64_t N, int64_t M)
            : linear(register_module("linear", torch::nn::Linear(N, M))) {
        another_bias = register_parameter("b", torch::randn(M));
    }

    torch::Tensor forward(torch::Tensor input) {
        return linear(input) + another_bias;
    }

    torch::nn::Linear linear;
    torch::Tensor another_bias;
};

TORCH_MODULE(Net);

int main() {
    Net net(4, 5);
    std::cout << net->forward(torch::ones({2, 4})) << std::endl;
}
