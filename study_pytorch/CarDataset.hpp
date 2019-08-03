//
// Created by Administrator on 2019/7/30.
//

#ifndef STUDY_CPP_CARDATASET_HPP
#define STUDY_CPP_CARDATASET_HPP

#include <random>
#include <boost/format.hpp>
#include <torch/torch.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

using namespace torch;

class CarDataItem {
public:
    CarDataItem(std::string filename, int brand, int type) : _filename(std::move(filename)), _brand(brand),
                                                             _type(type) {

    }

    const string &getFilename() const {
        return _filename;
    }

    int getBrand() const {
        return _brand;
    }

    int getType() const {
        return _type;
    }

private:
    std::string _filename{}; //like 1.jpg
    int _brand{};
    int _type{};

};

class CarNetImpl : public torch::nn::Module {
public:
    CarNetImpl(int ch, int classes) {
        //Layer 0
        _conv_0 = register_module("conv_0",
                                  nn::Conv2d(nn::Conv2dOptions(ch, 64, 4)
                                                     .stride(2)
                                                     .padding(1)
                                                     .with_bias(false)));
        _relu_0 = register_module("relu_0",
                                  nn::Functional(torch::leaky_relu, 0.2));

        //Layer 1
        _conv_1 = register_module("conv_1",
                                  nn::Conv2d(nn::Conv2dOptions(64, 128, 4)
                                                     .stride(2)
                                                     .padding(1)
                                                     .with_bias(false)));
        _bn_1 = register_module("bn_1",
                                nn::BatchNorm(nn::BatchNormOptions(128)));
        _relu_1 = register_module("relu_1",
                                  nn::Functional(torch::leaky_relu, 0.2));

        //Layer 2
        _conv_2 = register_module("conv_2",
                                  nn::Conv2d(nn::Conv2dOptions(128, 256, 4)
                                                     .stride(2)
                                                     .padding(1)
                                                     .with_bias(false)));
        _bn_2 = register_module("bn_2",
                                nn::BatchNorm(nn::BatchNormOptions(256)));
        _relu_2 = register_module("relu_2",
                                  nn::Functional(torch::leaky_relu, 0.2));

        //Layer 3
        _conv_3 = register_module("conv_3",
                                  nn::Conv2d(
                                          nn::Conv2dOptions(256, 1, 3)
                                                  .stride(2)
                                                  .padding(0)
                                                  .with_bias(false)));

        _dropout = register_module("dropout", nn::Dropout(nn::DropoutOptions(0.5)));
        _fc_3 = register_module("fc_3", nn::Linear(961, 1));
        _sigmoid = register_module("sigmoid", nn::Functional(torch::sigmoid));
    }

    torch::Tensor forward(torch::Tensor x) {
        //Layer 0
        x = _conv_0->forward(x);
        x = _relu_0->forward(x);
        //Layer 1
        x = _conv_1->forward(x);
        x = _bn_1->forward(x);
        x = _relu_1->forward(x);
        //Layer 2
        x = _conv_2->forward(x);
        x = _bn_2->forward(x);
        x = _relu_2->forward(x);
        //Layer 3
        x = _conv_3->forward(x);
        x = x.reshape({x.size(0), -1});
        x = _dropout->forward(x);
        x = _fc_3->forward(x);
        x = _sigmoid->forward(x);
        return x;
    }

private:
    // Layer 0
    torch::nn::Conv2d _conv_0{nullptr};
    torch::nn::Functional _relu_0{nullptr};
    // Layer 1
    torch::nn::Conv2d _conv_1{nullptr};
    torch::nn::BatchNorm _bn_1{nullptr};
    torch::nn::Functional _relu_1{nullptr};

    // Layer 2
    torch::nn::Conv2d _conv_2{nullptr};
    torch::nn::BatchNorm _bn_2{nullptr};
    torch::nn::Functional _relu_2{nullptr};

    // Layer 3
    torch::nn::Conv2d _conv_3{nullptr};
    torch::nn::Linear _fc_3{nullptr};
    torch::nn::Dropout _dropout{nullptr};

    torch::nn::Functional _sigmoid{nullptr};
};


TORCH_MODULE(CarNet);

class CarDataset : public torch::data::Dataset<CarDataset> {
public:
    explicit CarDataset(const std::string &path) {
        if (!boost::filesystem::exists(path + "/Label.TXT")) {
            throw std::runtime_error("Label.TXT not exists");
        }

        std::ifstream txt(path + "/Label.TXT");

        while (txt.good()) {
            std::string filename;
            int brand;
            int type;
            txt >> filename;
            txt >> brand;
            txt >> type;
            if (txt.eof())break;
            _data.emplace_back(path + "/" + filename, brand, type);
        }

        txt.close();

        std::shuffle(_data.begin(), _data.end(), std::mt19937(std::random_device()()));
    }


    optional<size_t> size() const override {
        return _data.size();
    };

    torch::data::Example<> get(size_t index) override {
        auto &item = _data.at(index);
        auto img = cv::imread(item.getFilename());
        if (img.empty()) {
            throw std::runtime_error((boost::format("image: %s is not valid") % item.getFilename()).str());
        }

        cv::resize(img, img, {512, 512});
        torch::Tensor image = torch::tensor(
                torch::ArrayRef < uint8_t > (img.data, img.rows * img.cols * 3))
                .view({3, img.rows, img.cols})
                .to(at::kFloat)
                .div(255);


//        torch::Tensor brand_label = torch::zeros({3}, torch::kFloat);
//        brand_label[item.getBrand()] = 1.f;

        torch::Tensor type_label = torch::zeros({1}, torch::kFloat);
        type_label[0] = item.getType();

        return {image.clone(), type_label.clone()};
    }


private:
    std::vector<CarDataItem> _data;
};


#endif //STUDY_CPP_CARDATASET_HPP
