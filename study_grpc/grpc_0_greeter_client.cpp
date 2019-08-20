//
// Created by linwei on 2019-08-20.
//

#include <grpcpp/grpcpp.h>
#include "greeter.grpc.pb.h"


class GreeterClient {
public:
    explicit GreeterClient(std::shared_ptr<grpc::Channel> channel)
            : stub_(greeter::Greeter::NewStub(channel)) {}

    std::string SayHelloAgain(const std::string &user) {
        greeter::HelloRequest request;
        request.set_name(user);

        greeter::HelloReply reply;

        grpc::ClientContext ctx;

        grpc::Status status = stub_->SayHelloAgain(&ctx, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ":" << status.error_message() << std::endl;
            return status.error_details();
        }
    }

    std::string SayHello(const std::string &user) {
        // Data we are sending to the server.
        greeter::HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        greeter::HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        grpc::ClientContext context;

        // The actual RPC.
        grpc::Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<greeter::Greeter::Stub> stub_{};
};

int main(int argc, char **argv) {
    GreeterClient greeter(grpc::CreateChannel(
            "localhost:50051", grpc::InsecureChannelCredentials()));

    std::string user("world");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;


    std::string again = greeter.SayHelloAgain(user);
    std::cout << "Greeter Again: " << again << std::endl;
    return 0;
}