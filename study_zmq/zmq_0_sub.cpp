//
// Created by linwei on 2019-08-12.
// vcpkg install nlohmann-json
// find_package(nlohmann_json CONFIG REQUIRED)
// target_link_libraries(main PRIVATE nlohmann_json nlohmann_json::nlohmann_json)


#include <iostream>
#include <string>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <glog/logging.h>

bool loop = true;

int main(int argc, char *argv[]) {
    std::string topic = "kismet_yb19";
    std::string proxy = "tcp://domain:5572";

    zmq::context_t ctx(1);
    zmq::socket_t sub(ctx, zmq::socket_type::sub);
//    zmq::socket_t sub(ctx, ZMQ_SUB); // or

    //set topic
    sub.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());

    int heartbeat_ivl = 1000;
    sub.setsockopt(ZMQ_HEARTBEAT_IVL, &heartbeat_ivl, sizeof(heartbeat_ivl));

    int rcvtimeo = 1000;//receive timeout
    sub.setsockopt(ZMQ_RCVTIMEO, &rcvtimeo, sizeof(rcvtimeo));


    //connect to proxy
    sub.connect(proxy);

    while (loop) {
        zmq::message_t t;
        zmq::message_t message;
        bool ret;
        ret = sub.recv(&t);
        if (!ret)continue;
        LOG(INFO) << "topic: " << t.str();
        ret = sub.recv(&message);
        if (ret) {
            //get message of string then parse for json
            try {
                nlohmann::json json = nlohmann::json::parse(std::string((char *) message.data(), message.size()));
                LOG(INFO) << json["hello"];

            } catch (std::exception &e) {
                LOG(INFO) << e.what();
            }
        }

    }


    return 0;
}