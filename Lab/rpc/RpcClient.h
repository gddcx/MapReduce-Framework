#pragma once

#include <grpcpp/grpcpp.h>
#include "mr.grpc.pb.h"
#include "mr.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using MapReduce::RequireJob;
using MapReduce::NodeMessage;
using MapReduce::Reply;
using MapReduce::FinishMsg;
using MapReduce::TaskType;

class MapReduceClient
{
private:
    std::unique_ptr<RequireJob::Stub> stub_;
public:
    MapReduceClient(std::string& target);
    bool Require(bool isBusy, MapReduce::TaskType& taskType, std::string& filename);
    void Finish(TaskType& taskType, std::string& filename);
};