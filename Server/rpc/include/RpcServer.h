#pragma once

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
using google::protobuf::Empty;

class RpcServer final: public JobRpc::Service
{
private:
    std::string rpcListen_;
    Status RequireJob(ServerContext* context, const NodeMessage* nodeMsg, JobMessage* jobMsg) override;
    Status FetchDataFromMap(ServerContext* context, const NodeMessage* nodeMsg, MapDataList* mapDataList) override;
    Status ReportJobStatus(ServerContext* context, const NodeMessage* nodeMsg, Empty* response) override;
public:
    RpcServer();
    RpcServer(std::string& rpcListen);
    void RunRpcServer();
private:
    std::function<bool(std::string, std::string&, std::string&)> GetMapJobCallback_;
    std::function<int()> GetReduceJobNumCallback_;
    std::function<void(std::string&)> ChangeWorkStatusCallback_;
    std::function<std::vector<std::string>(std::string)> GetIntermediateFileCallback_;
public:
    void SetGetMapJobCallback(std::function<bool(std::string, std::string&, std::string&)>);
    void SetGetReduceJobNumCallback(std::function<int()>);
    void SetChangeWorkStatusCallback(std::function<void(std::string&)>);
    void SetGetIntermediateFileCallback(std::function<std::vector<std::string>(std::string)>);
};