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
using masterSlaveRPC::JobMessage_TaskType;
using masterSlaveRPC::MapDataList;

class RpcClient
{
private:
    std::unique_ptr<JobRpc::Stub> stub_;
public:
    RpcClient(std::string& target);
    bool Require(NodeMessage& nodeMsg, JobMessage& jobMsg);
    void JobFinished(NodeMessage& nodeMsg);
    bool Fetch(NodeMessage& nodeMsg, MapDataList& mapDataList);
};