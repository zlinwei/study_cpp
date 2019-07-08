//
// Created by Administrator on 2019/7/8.
//

#include <vector>
#include <algorithm>
#include <chrono>

#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>

#include <glog/logging.h>

namespace compute = boost::compute;

int main() {
    const size_t size = 200000000;
    // get default device and setup context
    compute::device device = compute::system::default_device();
    LOG(INFO) << "Device: " << device.name();
    compute::context context(device);
    compute::command_queue queue(context, device);

    // generate random data on the host
    std::vector<float> host_vector(size);
    std::generate(host_vector.begin(), host_vector.end(), rand);

    // create a vector on the device
    compute::vector<float> device_vector(host_vector.size(), context);

    auto start = std::chrono::high_resolution_clock::now();
    // transfer data from the host to the device
    compute::copy(
            host_vector.begin(), host_vector.end(), device_vector.begin(), queue
    );

    // calculate the square-root of each element in-place
    compute::transform(
            device_vector.begin(),
            device_vector.end(),
            device_vector.begin(),
            compute::sqrt<float>(),
            queue
    );

    // copy values back to the host
    compute::copy(
            device_vector.begin(), device_vector.end(), host_vector.begin(), queue
    );


    auto elapse = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start
    );
    LOG(INFO) << "elapse: " << elapse.count() << " ms";


    // host compute
    std::generate(host_vector.begin(), host_vector.end(), rand);

    start = std::chrono::high_resolution_clock::now();


    for (auto &item : host_vector) {
        item = std::sqrt(item);
    }

    elapse = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start
    );
    LOG(INFO) << "elapse: " << elapse.count() << " ms";


    return 0;
}