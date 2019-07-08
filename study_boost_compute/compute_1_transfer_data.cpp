//
// Created by Administrator on 2019/7/8.
//

#include <vector>

#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include "glog/logging.h"

namespace compute = boost::compute;

int main() {
    // get default device and setup context
    compute::device device = compute::system::default_device();
    compute::context context(device);
    compute::command_queue queue(context, device);

    // create data array on host
    std::vector<int> host_data = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};

    // create vector on device
    compute::vector<int> device_vector(host_data.size(), context);

    // copy from host to device
    compute::copy(
            host_data.begin(), host_data.end(), device_vector.begin(), queue
    );

    // create vector on host
    std::vector<int> host_vector(host_data.size());

    // copy data back to host
    compute::copy(
            device_vector.begin(), device_vector.end(), host_vector.begin(), queue
    );

    for (int i = 0; i < device_vector.size(); ++i) {
        LOG(INFO) << "device_vector[" << i << "]=" << device_vector.at(i);
    }

    return 0;
}