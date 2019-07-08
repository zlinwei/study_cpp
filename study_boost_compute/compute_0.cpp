//
// Created by Administrator on 2019/7/8.
//

#include <iostream>
#include <boost/compute/core.hpp>

#include <glog/logging.h>

namespace compute = boost::compute;

int main() {
    // get the device count
    LOG(INFO) << "Dev Count: " << compute::system::device_count();
    for (auto &dev : compute::system::devices()) {
        LOG(INFO) << dev.name() << "(platform: " << dev.platform().name() << ")" << " Dev ID: " << dev.id();
    }

    // get the default device
    compute::device device = compute::system::default_device();

    // print the device's name and platform
    LOG(INFO) << "Default: " << device.name();
    LOG(INFO) << "Default Device ID: " << device.id();
    return 0;
}