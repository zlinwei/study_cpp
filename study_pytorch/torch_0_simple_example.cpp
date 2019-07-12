//
// Created by Administrator on 2019/7/12.
//

#include <torch/torch.h>
#include <iostream>

/*
 * In Python, it would look like this:
 import torch
 class Net(torch.nn.Module):
    def __init__(self, N, M):
        super(Net, self).__init__()
        self.W = torch.nn.Parameter(torch.randn(N, M))
        self.b = torch.nn.Parameter(torch.randn(M))

    def forward(self, input):
        return torch.addmm(self.b, input, self.W)
*/

struct Net : torch::nn::Module {
    Net(int64_t N, int64_t M) {
        W = register_parameter("W", torch::randn({N, M}));
        b = register_parameter("b", torch::randn(M));
    }

    torch::Tensor forward(torch::Tensor input) {
        return torch::addmm(b, input, W);
    }

    torch::Tensor W, b;
};

int main() {
    torch::Tensor tensor = torch::eye(3);
    std::cout << tensor << std::endl;
	
    return 0;
}


