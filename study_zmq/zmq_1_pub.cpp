//
// Created by linwei on 2019-08-12.
// vcpkg install nlohmann-json
// find_package(nlohmann_json CONFIG REQUIRED)
// target_link_libraries(main PRIVATE nlohmann_json nlohmann_json::nlohmann_json)


#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <glog/logging.h>

bool loop = true;

int main(int argc, char *argv[]) {
    std::string topic = "kismet_yb19";
    std::string proxy = "tcp://domain:5571";

    zmq::context_t ctx(1);
    zmq::socket_t pub(ctx, zmq::socket_type::pub);
//    zmq::socket_t pub(ctx, ZMQ_PUB); // or



    //connect to proxy
    pub.connect(proxy);
    LOG(INFO) << "connect";

    std::string hello = R"({"hello":"world"})";
    while (loop) {
        pub.send(topic.data(), topic.size(), ZMQ_SNDMORE);
        pub.send(hello.data(), hello.size());
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }


    return 0;
}