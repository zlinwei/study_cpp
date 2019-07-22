//
// Created by linwei on 2019-07-19.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>

#include <boost/format.hpp>
#include <torch/torch.h>
#include <glog/logging.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "CSVRow.hpp"

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
        LOG(INFO) << "read csv: " << loc;
        std::ifstream ifile(loc + "train.csv");
        if (ifile.fail())throw std::runtime_error((boost::format("can not open file %s") % loc).str());

        CSVRow row;
        while (ifile >> row) {
            std::string filename = loc + "train/" + row[0] + "_green.png";
            //read image
            auto img_ = cv::imread(filename, cv::IMREAD_GRAYSCALE);
            if (img_.empty())continue;
            cv::Mat img(512, 512, CV_8UC1);
            cv::resize(img_, img, img.size(), 0, 0, cv::INTER_AREA);
            auto image = torch::tensor(torch::ArrayRef < uint8_t > (img.data, img.rows * img.cols * 1)).view(
                    {img.rows, img.cols, 1});

            //read label
            torch::Tensor label = torch::zeros({28});
            std::stringstream ss;
            ss << row[1];
            while (true) {
                int tmp;
                ss >> tmp;
                if (ss.eof())break;
                label[tmp] = 1;
            }
//            cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
//            cv::imshow("Display window", img);
//            cv::waitKey();

            _states.push_back(image);
            _labels.push_back(label);
        }
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
    std::string path = "/Users/linwei/data/human_protein_atlas/";
    auto dataset = HumanProteinAtlasDataset(path);
    return 0;
}