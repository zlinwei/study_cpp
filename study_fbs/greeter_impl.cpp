//
// Created by linwei on 2020/4/30.
//

#include "greeter_impl.h"

GreeterImpl::~GreeterImpl() {

}

::grpc::Status GreeterImpl::SayHello(::grpc::ServerContext *context, flatbuffers::grpc::Message<HelloRequest> *request,
                                     flatbuffers::grpc::Message<HelloReply> *response) {
    return Service::SayHello(context, request, response);
}

::grpc::Status
GreeterImpl::SayManyHellos(::grpc::ServerContext *context, flatbuffers::grpc::Message<ManyHellosRequest> *request,
                           ::grpc::ServerWriter<flatbuffers::grpc::Message<HelloReply>> *writer) {
    return Service::SayManyHellos(context, request, writer);
}
