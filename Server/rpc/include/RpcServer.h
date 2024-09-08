#pragma once

#include <queue>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "rpc.grpc.pb.h"
#include "public.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using masterSlaveRPC::JobRpc;
using masterSlaveRPC::NodeMessage;
using masterSlaveRPC::JobMessage;
using masterSlaveRPC::EventMessage;
using masterSlaveRPC::JobMessage_TaskType;
using masterSlaveRPC::MapDataList;

class RpcServer final: public JobRpc::Service
{
private:
    std::string rpcListen_;
    std::queue<std::string> jobs_;
    std::mutex jobsMutex_;
    Status RequireJob(ServerContext* context, const NodeMessage* nodeMsg, JobMessage* jobMsg) override;
    Status FetchDataFromMap(ServerContext* context, const NodeMessage* nodeMsg, MapDataList* mapDataList) override;
    std::string GetJob();
    uint GetJobsNum();
public:
    RpcServer();
    RpcServer(std::string& rpcListen);
    void RunRpcServer();
    void AddJob(std::string& filename);
};