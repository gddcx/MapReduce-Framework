#pragma once

#include <queue>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "mr.grpc.pb.h"
#include "public.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using MapReduce::RequireJob;
using MapReduce::NodeMessage;
using MapReduce::Reply;

class MapReduceServer final: public RequireJob::Service
{
private:
    std::string rpcListen_;
    std::queue<std::string> jobs_;
    std::mutex jobsMutex_;
    Status Require(ServerContext* context, const NodeMessage* nodeMsg, Reply* reply) override;
    std::string GetJob();
    uint GetJobsNum();
public:
    MapReduceServer();
    MapReduceServer(std::string& rpcListen);
    void RunRpcServer();
    void AddJob(std::string& filename);
};