#include "RpcServer.h"

MapReduceServer::MapReduceServer()
{
    std::cout << "no specific ip and port to start RPC server" << std::endl;
    std::cout << "use default 0.0.0.0:50031" << std::endl;
    rpcListen_ = std::string("0.0.0.0:50031");
}

MapReduceServer::MapReduceServer(std::string& rpcListen): rpcListen_(rpcListen) {}

Status MapReduceServer::Require(ServerContext* context, const NodeMessage* nodeMsg, Reply* reply)
{
    if((nodeMsg->isbusy() == NON_BUSY) && (GetJobsNum() != 0))
    {
        reply->set_filename(GetJob());
    }
    else /* 客户端忙或者服务端没活，就只记录心跳 */
    {

    }

    return Status::OK;
}

void MapReduceServer::RunRpcServer()
{
    ServerBuilder builder;

    builder.AddListeningPort(rpcListen_, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    server->Wait();
}

void MapReduceServer::AddJob(std::string& filename)
{
    std::lock_guard<std::mutex> lock(jobsMutex_);
    jobs_.push(filename);
}

std::string MapReduceServer::GetJob()
{
    std::lock_guard<std::mutex> lock(jobsMutex_);
    std::string job = jobs_.front();
    jobs_.pop();

    return job;
}

uint MapReduceServer::GetJobsNum()
{
    return jobs_.size();
}