//
// Created by linwei on 2019-07-19.
//

#include <iostream>
#include <torch/torch.h>
#include <glog/logging.h>

class HumanProteinAtlasDataset :
        public torch::data::Dataset<HumanProteinAtlasDataset> {
public:
    explicit HumanProteinAtlasDataset(const std::string &loc) {
        read_data(loc);
    }

    torch::data::Example<> get(size_t index) override;

    c10::optional<size_t> size() const override;

private:
    void read_data(const std::string &loc) {
        //TODO read data into states and labels

    }


private:
    std::vector<torch::Tensor> _states;
    std::vector<torch::Tensor> _labels;
};

torch::data::Example<> HumanProteinAtlasDataset::get(size_t index) {
    return {_states.at(index), _labels.at(index)};
}

c10::optional<size_t> HumanProteinAtlasDataset::size() const {
    return std::min<size_t>(_states.size(), _labels.size());
}

int main(int, char *[]) {
//    torch::Tensor tensor = torch::ones({2, 3, 3, 3});
//    LOG(INFO) << tensor[0];
//    LOG(INFO) << tensor[1];
//    LOG(INFO) << tensor[2];
//    tensor[3] = torch::ones({3, 3, 3}); // will throw an exception of out of range


    return 0;
}