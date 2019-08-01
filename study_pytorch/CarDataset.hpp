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

class CarNet : public torch::nn::Module {
public:
    CarNet(int ch, int classes) {
        _conv0 = register_module("conv_0", nn::Conv2d(nn::Conv2dOptions(ch, 20, {5, 5})));
        _conv1 = register_module("conv_1", nn::Conv2d(nn::Conv2dOptions(20, 40, {4, 4})));
        _conv2 = register_module("conv_2", nn::Conv2d(nn::Conv2dOptions(40, 60, {3, 3})));
        _fc0 = register_module("fc0", nn::Linear(60, 100));
        _fc1 = register_module("fc1", nn::Linear(100, classes));
    }

    torch::Tensor forward(torch::Tensor x) {
        x = _conv0->forward(x);
        x = torch::relu(x);
        x = torch::max_pool2d(x, {2, 2}, {2, 2});
        x = _conv1->forward(x);
        x = torch::relu(x);
        x = torch::max_pool2d(x, {2, 2}, {2, 2});
        x = _conv2->forward(x);
        x = torch::relu(x);
        x = torch::max_pool2d(x, {2, 2}, {2, 2});
        x = torch::adaptive_avg_pool2d(x, {1, 1});
        x = x.view({x.size(0), x.size(1)});
        x = _fc0->forward(x);
        x = torch::dropout(x, 0.5, is_training());
        x = _fc1->forward(x);
        x = torch::argmax(x, 1);
        return x;
    }

private:
    torch::nn::Conv2d _conv0{nullptr}; //chan=20,kernel_size=5,relu, max_pool 2x2 2x2
    torch::nn::Conv2d _conv1{nullptr}; //chan=40,kernel_size=4,relu, max_pool 2x2 2x2
    torch::nn::Conv2d _conv2{nullptr}; //chan=60,kernel_size=3,relu, max_pool 2x2 2x2
    torch::nn::Linear _fc0{nullptr}; //output=100,dropout
    torch::nn::Linear _fc1{nullptr};

};


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


        torch::Tensor brand_label = torch::zeros({3}, torch::kFloat);
        brand_label[item.getBrand()] = 1.0;
        torch::Tensor type_label = torch::zeros({2}, torch::kFloat);
        type_label[item.getType()] = 1.0;

        return {image.clone(), type_label.clone()};

    }


private:
    std::vector<CarDataItem> _data;
};


#endif //STUDY_CPP_CARDATASET_HPP
