#include "RpcServer.h"

RpcServer::RpcServer()
{
    std::cout << "no specific ip and port to start RPC server" << std::endl;
    std::cout << "use default 0.0.0.0:50031" << std::endl;
    rpcListen_ = std::string("0.0.0.0:50031");
}

RpcServer::RpcServer(std::string& rpcListen): rpcListen_(rpcListen) {}

Status RpcServer::RequireJob(ServerContext* context, const NodeMessage* nodeMsg, JobMessage* jobMsg)
{
    if((nodeMsg->nodename() != "") && (GetJobsNum() != 0))
    {
        jobMsg->set_type(masterSlaveRPC::JobMessage_TaskType_map);
    }
    else /* 客户端忙或者服务端没活，就只记录心跳 */
    {

    }

    return Status::OK;
}

Status RpcServer::FetchDataFromMap(ServerContext* context, const NodeMessage* nodeMsg, MapDataList* mapDataList)
{
    mapDataList->add_filename("intermedia-0.bin");
    mapDataList->add_filename("intermedia-1.bin");
    return Status::OK;
}

void RpcServer::RunRpcServer()
{
    ServerBuilder builder;

    builder.AddListeningPort(rpcListen_, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<Server> server(builder.BuildAndStart());

    server->Wait();
}

/* 不合理，为什么是RpcServer保存任务，应该是master负责控制任务 */
void RpcServer::AddJob(std::string& filename)
{
    std::lock_guard<std::mutex> lock(jobsMutex_);
    jobs_.push(filename);
}

std::string RpcServer::GetJob()
{
    std::lock_guard<std::mutex> lock(jobsMutex_);
    std::string job = jobs_.front();
    jobs_.pop();

    return job;
}

uint RpcServer::GetJobsNum()
{
    return jobs_.size();
}