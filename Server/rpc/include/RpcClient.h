#pragma once

#include <grpcpp/grpcpp.h>
#include "rpc.grpc.pb.h"
#include "rpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using masterSlaveRPC::JobRpc;
using masterSlaveRPC::NodeMessage;
using masterSlaveRPC::JobMessage;
using masterSlaveRPC::EventMessage;
using masterSlaveRPC::TaskType;
using masterSlaveRPC::MapDataList;
using masterSlaveRPC::Id;

class RpcClient
{
private:
    std::unique_ptr<JobRpc::Stub> stub_;
public:
    RpcClient(std::string& target);
    bool Require(NodeMessage& nodeMsg, JobMessage& jobMsg);
    void JobFinished(JobMessage& jobMsg);
    void HeartBeat(NodeMessage& nodeMsg);
};