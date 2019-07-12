//
// Created by Administrator on 2019/7/12.
//

#include <iostream>

#include <torch/torch.h>

using namespace torch;

class Net : public nn::Module
{
public:
	Net() {
		// 1 input image channel, 6 output channels, 3x3 square convolution
		// kernel
		_conv1 = register_module("conv1", nn::Conv2d(nn::Conv2dOptions(1, 6, 3)));
		_conv2 = register_module("conv2", nn::Conv2d(nn::Conv2dOptions(6, 16, 3)));
		// an affine operation: y = Wx + b
		_fc1 = register_module("fc1", nn::Linear(16 * 6 * 6, 120));
		_fc2 = register_module("fc2", nn::Linear(120, 84));
		_fc3 = register_module("fc3", nn::Linear(84, 10));
	}


private:
	nn::Conv2d _conv1 = nullptr;
	nn::Conv2d _conv2 = nullptr;
	nn::Linear _fc1 = nullptr;
	nn::Linear _fc2 = nullptr;
	nn::Linear _fc3 = nullptr;
};



int main() {
	auto net = std::make_shared<Net>();
	std::cout << *net << std::endl;
	return 0;
}