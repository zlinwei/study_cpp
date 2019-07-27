//
// Created by linwei on 2019-07-22.
//

#ifndef STUDY_CPP_HUMANPROTEINDATASETS_HPP
#define STUDY_CPP_HUMANPROTEINDATASETS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
#include <random>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <torch/torch.h>
#include <glog/logging.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "CSVRow.hpp"

class HumanProteinDataItem {
private:
    std::string _filename;
public:
    const std::string &getFilename() const {
        return _filename;
    }

    const std::string &getTarget() const {
        return _target;
    }

private:
    std::string _target;
public:
    HumanProteinDataItem(std::string filename, std::string target) : _filename(std::move(filename)),
                                                                     _target(std::move(target)) {

    }
};

class HumanProteinAtlasDataset :
        public torch::data::Dataset<HumanProteinAtlasDataset> {
public:
    explicit HumanProteinAtlasDataset(const std::string &loc, size_t size = 0){
        read_data(loc,size);
		std::shuffle(_data.begin(), _data.end(), std::mt19937(std::random_device()()));
    }

    torch::data::Example<> get(size_t index) override;

    c10::optional<size_t> size() const override;

private:
    void read_data(const std::string &loc,size_t size) {
        //TODO read data into states and labels
        LOG(INFO) << "read csv: " << loc;
        std::ifstream ifile(loc + "/train.csv");
        if (ifile.fail())throw std::runtime_error((boost::format("can not open file %s") % loc).str());

        CSVRow row;
		size_t tmp = size;
        while (ifile >> row) {
            std::string filename = loc + "/train/" + row[0] + "_green.png";
            if (!boost::filesystem::exists(filename)) continue;
            _data.emplace_back<HumanProteinDataItem>({filename, row[1]});
            //LOG(INFO) << filename << " " << row[1];
			if (size != 0 && --tmp <= 0)break;
        }
    }


private:
    std::vector<HumanProteinDataItem> _data;
};

torch::data::Example<torch::Tensor, torch::Tensor> HumanProteinAtlasDataset::get(size_t index) {
    const auto &item = _data.at(index);
    //read image
    auto img_ = cv::imread(item.getFilename(), cv::IMREAD_GRAYSCALE);
    if (img_.empty())
        throw std::runtime_error((boost::format("image: %s is not valid") % item.getFilename()).str());

    auto image = torch::tensor(torch::ArrayRef < uint8_t > (img_.data, img_.rows * img_.cols * 1)).view(
            {1, img_.rows, img_.cols}).to(at::kFloat).div(255);


    //read label
    torch::Tensor label = torch::zeros({28}).to(at::kFloat);
    std::stringstream ss;
    ss << item.getTarget();
    while (true) {
        int tmp;
        ss >> tmp;
        if (ss.eof())break;
        label[tmp] = 1;
    }
    return { image.clone(), label.clone()};
}

c10::optional<size_t> HumanProteinAtlasDataset::size() const {
    return _data.size();
}


#endif //STUDY_CPP_HUMANPROTEINDATASETS_HPP
