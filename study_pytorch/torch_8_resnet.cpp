#include <utility>

#include <utility>

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

#include "HumanProteinDatasets.hpp"

using namespace torch;

int main(int, char *[]) {

    nn::Sequential resnet18(
            nn::Conv2d(nn::Conv2dOptions(1, 64, 4).stride(2).padding(1).with_bias(false)),
            nn::Functional(torch::leaky_relu, 0.2)
    );

    std::string path = "/Users/linwei/data/human_protein_atlas/";
    auto dataset = HumanProteinAtlasDataset(path);
    // data loader
    auto data_loader = torch::data::make_data_loader(
            std::move(dataset),
            torch::data::DataLoaderOptions().batch_size(64).workers(2));


    return 0;
}