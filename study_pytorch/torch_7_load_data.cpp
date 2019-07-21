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


class CSVRow {
public:
    std::string const &operator[](std::size_t index) const {
        return m_data.at(index);
    }

    std::size_t size() const {
        return m_data.size();
    }

    void readNextRow(std::istream &str) {
        std::string line;
        std::getline(str, line);

        std::stringstream lineStream(line);
        std::string cell;

        m_data.clear();
        while (std::getline(lineStream, cell, ',')) {
            m_data.push_back(cell);
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty()) {
            // If there was a trailing comma then add an empty element.
            m_data.emplace_back("");
        }
    }

private:
    std::vector<std::string> m_data;
};

std::istream &operator>>(std::istream &str, CSVRow &data) {
    data.readNextRow(str);
    return str;
}

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
            std::string target = row[1];
            std::string filename = loc + "train/" + row[0] + "_green.png";
            auto img_ = cv::imread(filename, cv::IMREAD_GRAYSCALE);
            if (img_.empty())continue;
            cv::Mat img(512, 512, CV_8UC1);
            cv::resize(img_, img, img.size(), 0, 0, cv::INTER_AREA);
            LOG(INFO) << "rows: " << img.rows << " cols: " << img.cols;
            auto image = torch::tensor(torch::ArrayRef < uint8_t > (img.data, img.rows * img.cols * 1)).view(
                    {img.rows, img.cols, 1});

            torch::Tensor label = torch::zeros({1, 28});

            cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
            cv::imshow("Display window", img);
            cv::waitKey();

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