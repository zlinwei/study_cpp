//
// Created by Administrator on 2019/7/30.
//

#ifndef STUDY_CPP_CARDATASET_HPP
#define STUDY_CPP_CARDATASET_HPP

#include <torch/torch.h>
#include <opencv2/opencv.hpp>
using namespace torch;

class CarDataItem{
public:
    const string &getFilename() const {
        return _filename;
    }

    const string &getLabelStr() const {
        return _label_str;
    }

private:
    std::string _filename; //like 1.jpg
    std::string _label_str; //like 0 0
};

class CarDataset :public torch::data::Dataset<CarDataset>{
public:
    optional<size_t> size() const override {
        return _data.size();
    }

    ExampleType get(size_t index) override {
        auto &item = _data.at(index);
        auto img = cv::imread(item.getFilename());

    }


private:
    std::vector<CarDataItem> _data;
};


#endif //STUDY_CPP_CARDATASET_HPP
