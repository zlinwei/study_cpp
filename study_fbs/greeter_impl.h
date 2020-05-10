//
// Created by linwei on 2020/4/30.
//

#ifndef STUDY_CPP_GREETER_IMPL_H
#define STUDY_CPP_GREETER_IMPL_H

#include "greeter_generated.h"
#include "greeter.grpc.fb.h"
#include <grpc++/grpc++.h>

class GreeterImpl :public Greeter::Service{
public:
    ~GreeterImpl() override;

    ::grpc::Status SayHello(::grpc::ServerContext *context, flatbuffers::grpc::Message<HelloRequest> *request,
                            flatbuffers::grpc::Message<HelloReply> *response) override;

    ::grpc::Status SayManyHellos(::grpc::ServerContext *context, flatbuffers::grpc::Message<ManyHellosRequest> *request,
                                 ::grpc::ServerWriter<flatbuffers::grpc::Message<HelloReply>> *writer) override;
};


#endif //STUDY_CPP_GREETER_IMPL_H
