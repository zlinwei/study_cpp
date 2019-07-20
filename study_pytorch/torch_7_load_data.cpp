//
// Created by linwei on 2019-07-19.
//

#include <iostream>
#include <torch/torch.h>

class HumanProteinAtlasDataset :
        public torch::data::Dataset<HumanProteinAtlasDataset> {
public:
    explicit HumanProteinAtlasDataset(const std::string &loc) {
        read_data(loc);
    }

    torch::data::Example<> get(size_t index) override;

private:
    void read_data(const std::string &loc) {
        //TODO read data into states and labels
    }


private:
    torch::Tensor _states;
    torch::Tensor _labels;
};

torch::data::Example<> HumanProteinAtlasDataset::get(size_t index) {
    return {_states[index], _labels[index]};
}

int main(int, char *[]) {


    return 0;
}