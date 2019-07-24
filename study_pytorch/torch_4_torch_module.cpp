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


struct GeneratorImpl : torch::nn::Module {
    GeneratorImpl()
            : conv1(torch::nn::Conv2dOptions(100, 512, 4)
                            .with_bias(false)
                            .transposed(true)),
              batch_norm1(512),
              conv2(torch::nn::Conv2dOptions(512, 256, 4)
                            .stride(2)
                            .padding(1)
                            .with_bias(false)
                            .transposed(true)),
              batch_norm2(256),
              conv3(torch::nn::Conv2dOptions(256, 128, 4)
                            .stride(2)
                            .padding(1)
                            .with_bias(false)
                            .transposed(true)),
              batch_norm3(128),
              conv4(torch::nn::Conv2dOptions(128, 64, 4)
                            .stride(2)
                            .padding(1)
                            .with_bias(false)
                            .transposed(true)),
              batch_norm4(64),
              conv5(torch::nn::Conv2dOptions(64, 1, 4)
                            .stride(2)
                            .padding(1)
                            .with_bias(false)
                            .transposed(true)) {}

    torch::Tensor forward(torch::Tensor x) {
        x = torch::relu(batch_norm1(conv1(x)));
        x = torch::relu(batch_norm2(conv2(x)));
        x = torch::relu(batch_norm3(conv3(x)));
        x = torch::relu(batch_norm4(conv4(x)));
        x = torch::tanh(conv5(x));
        return x;
    }

    torch::nn::Conv2d conv1, conv2, conv3, conv4, conv5;
    torch::nn::BatchNorm batch_norm1, batch_norm2, batch_norm3, batch_norm4;
};

TORCH_MODULE(Generator);


int main() {
    Generator generator{};
    std::cout << generator << std::endl;
    Net net(100, 5);
    std::cout << net << std::endl;
    std::cout << net->forward(torch::ones({30, 1, 100})) << std::endl;
}
