//
// Created by linwei on 2019-08-20.
//
#include <grpcpp/grpcpp.h>
#include "greeter.pb.h"
#include "greeter.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class GreeterServiceImpl final : public greeter::Greeter::Service {
public:
    grpc::Status SayHello(::grpc::ServerContext *context, const ::greeter::HelloRequest *request,
                          ::greeter::HelloReply *response) override {

        std::cout << request->name() << std::endl;
        response->set_message("world");
        return grpc::Status::OK;
    }

    grpc::Status SayHelloAgain(::grpc::ServerContext *context, const ::greeter::HelloRequest *request,
                               ::greeter::HelloReply *response) override {
        std::cout << request->name() << std::endl;
        response->set_message("Hi");
        return grpc::Status::OK;
    }


};



void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int, char *[]) {
    RunServer();

    return 0;
}