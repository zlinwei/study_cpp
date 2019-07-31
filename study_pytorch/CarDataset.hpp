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

class CarNet : public torch::nn::Module{

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
