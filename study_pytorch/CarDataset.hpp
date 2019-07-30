//
// Created by Administrator on 2019/7/30.
//

#ifndef STUDY_CPP_CARDATASET_HPP
#define STUDY_CPP_CARDATASET_HPP

#include <torch/torch.h>

using namespace torch;

class CarDataItem{
public:

};

class CarDataset :public torch::data::Dataset<CarDataset>{
public:
    optional<size_t> size() const override {
        return _data.size();
    }

    ExampleType get(size_t index) override {
        
    }


private:
    std::vector<CarDataItem> _data;
};


#endif //STUDY_CPP_CARDATASET_HPP
