//
// Created by Administrator on 2019/7/30.
//

#ifndef STUDY_CPP_RESNET18_HPP
#define STUDY_CPP_RESNET18_HPP

#include <torch/torch.h>

using namespace torch;

/**
 * conv1 7x7,64,stride 2
 */


class ConvBlock2Impl : public torch::nn::Module {
public:
    ConvBlock2Impl(int input_ch, int output_ch, int kernel_size) :
            _conv1(register_module("conv1", nn::Conv2d(
                    nn::Conv2dOptions(input_ch, output_ch, {kernel_size, kernel_size}).stride(1))
            )),
            _bn1(register_module("bn1",
                                 nn::BatchNorm(output_ch)
            )),
            _conv2(register_module("conv2", nn::Conv2d(
                    nn::Conv2dOptions(output_ch, output_ch, {kernel_size, kernel_size})
                            .stride(1))
            )),
            _bn2(register_module("bn2", nn::BatchNorm(output_ch))) {

    }

    torch::Tensor forward(torch::Tensor &x) {
        x = _conv1->forward(x);
        x = _bn1->forward(x);
        x = torch::relu(x);

        x = _conv2->forward(x);
        x = _bn2->forward(x);
        x = torch::relu(x);

        return x;
    }

private:
    nn::Conv2d _conv1{nullptr};
    nn::BatchNorm _bn1{nullptr};
    nn::Conv2d _conv2{nullptr};
    nn::BatchNorm _bn2{nullptr};
};

TORCH_MODULE(ConvBlock2);

class ResNet18Impl : public torch::nn::Module {
public:
    ResNet18Impl() :
            _conv1(register_module("conv1", nn::Conv2d(nn::Conv2dOptions(1, 64, {7, 7}).stride(2)))),
            _conv2_x(register_module("conv2_x", ConvBlock2(64, 64, 3))),
            _conv3_x(register_module("conv3_x", ConvBlock2(64, 128, 3))),
            _conv4_x(register_module("conv4_x", ConvBlock2(128, 256, 3))),
            _conv5_x(register_module("conv5_x", ConvBlock2(256, 512, 3))),
            _linear(register_module("linear", nn::Linear(512, 28))) {

    }

    Tensor forward(Tensor x) {
        x = _conv1->forward(x); //top layer
        x = torch::max_pool2d(x, 3, 2); //max pool2d
        x = _conv2_x->forward(x);
        x = _conv3_x->forward(x);
        x = _conv4_x->forward(x);
        x = _conv5_x->forward(x);
        x = torch::adaptive_avg_pool2d(x, {1, 1});
        x = x.view({x.size(0), x.size(1)});
        x = _linear->forward(x);
        x = torch::softmax(x, 1);

        return x;
    }

private:
    nn::Conv2d _conv1{nullptr};
    ConvBlock2 _conv2_x{nullptr};
    ConvBlock2 _conv3_x{nullptr};
    ConvBlock2 _conv4_x{nullptr};
    ConvBlock2 _conv5_x{nullptr};
    nn::Linear _linear{nullptr};

};

TORCH_MODULE(ResNet18);


#endif //STUDY_CPP_RESNET18_HPP
