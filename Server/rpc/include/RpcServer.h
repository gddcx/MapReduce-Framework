#pragma once

#include <grpcpp/grpcpp.h>
#include "rpc.grpc.pb.h"
#include "public.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;
using masterSlaveRPC::JobRpc;
using masterSlaveRPC::NodeMessage;
using masterSlaveRPC::JobMessage;
using masterSlaveRPC::EventMessage;
using masterSlaveRPC::TaskType;
using masterSlaveRPC::MapDataList;
using masterSlaveRPC::Id;
using google::protobuf::Empty;

class RpcServer final: public JobRpc::Service
{
private:
    std::string rpcListen_;
    Status RequireJob(ServerContext* context, const NodeMessage* nodeMsg, JobMessage* jobMsg) override;
    Status ReportJobStatus(ServerContext* context, const JobMessage* nodeMsg, Empty* response) override;
    Status HeartBeat(ServerContext* context, const NodeMessage* nodeMsg, Empty* response) override;
public:
    RpcServer();
    RpcServer(std::string& rpcListen);
    void RunRpcServer();
private:
    std::function<bool(std::string, std::string&, std::string&, uint&, uint&, uint&)> GetMapJobCallback_;
    std::function<bool(std::string, std::string&, std::string&, uint&, uint&, uint&)> GetReduceJobCallback_;
    std::function<void(int, uint, uint)> ChangeWorkStatusCallback_;
    std::function<std::vector<std::string>(std::string)> GetIntermediateFileCallback_;
    std::function<void(std::string&)> HeartBeatCallback_;
public:
    void SetGetMapJobCallback(std::function<bool(std::string, std::string&, std::string&, uint&, uint&, uint&)>);
    void SetGetReduceJobCallback(std::function<bool(std::string, std::string&, std::string&, uint&, uint&, uint&)>);
    void SetChangeJobStatusCallback(std::function<void(int, uint, uint)>);
    void SetGetIntermediateFileCallback(std::function<std::vector<std::string>(std::string)>);
    void SetHeartBeatCallback(std::function<void(std::string&)>);
};