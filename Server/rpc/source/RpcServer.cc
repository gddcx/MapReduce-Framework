#include "RpcServer.h"

RpcServer::RpcServer()
{
    std::cout << "no specific ip and port to start RPC server" << std::endl;
    std::cout << "use default 0.0.0.0:50031" << std::endl;
    rpcListen_ = std::string("0.0.0.0:50031");
}

RpcServer::RpcServer(std::string& rpcListen): rpcListen_(rpcListen){}

void RpcServer::RunRpcServer()
{
    ServerBuilder builder;

    builder.AddListeningPort(rpcListen_, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<Server> server(builder.BuildAndStart());

    server->Wait();
}

Status RpcServer::RequireJob(ServerContext* context, const NodeMessage* nodeMsg, JobMessage* jobMsg)
{
    std::string key;
    std::string value;
    if(GetMapJobCallback_(nodeMsg->nodename(), key, value) == MR_OK) // 有map工作未启动
    {
        jobMsg->set_key(key);
        jobMsg->set_value(value);
        jobMsg->set_type(masterSlaveRPC::JobMessage_TaskType_map);
        return Status::OK;
    }
    else if(GetReduceJobNumCallback_() > 0)
    {
        jobMsg->set_type(masterSlaveRPC::JobMessage_TaskType_reduce);
        return Status::OK;
    }
    else
    {
        
    }

    return Status(StatusCode::UNAVAILABLE, "no available job!");
}

Status RpcServer::ReportJobStatus(ServerContext* context, const NodeMessage* nodeMsg, Empty* response)
{
    std::string nodeName = nodeMsg->nodename();
    ChangeWorkStatusCallback_(nodeName);
    return Status::OK;
}

Status RpcServer::FetchDataFromMap(ServerContext* context, const NodeMessage* nodeMsg, MapDataList* mapDataList)
{
    std::stringstream url;

    for(const auto& url: GetIntermediateFileCallback_(nodeMsg->nodename()))
    {
        mapDataList->add_filename(url);
    }

    return Status::OK;
}

void RpcServer::SetGetMapJobCallback(std::function<bool(std::string, std::string&, std::string&)> GetMapJobCallback)
{
    GetMapJobCallback_ = GetMapJobCallback;
}

void RpcServer::SetGetReduceJobNumCallback(std::function<int()> GetReduceJobNumCallback)
{
    GetReduceJobNumCallback_ = GetReduceJobNumCallback;
}

void RpcServer::SetChangeWorkStatusCallback(std::function<void(std::string&)> ChangeWorkStatusCallback)
{
    ChangeWorkStatusCallback_ = ChangeWorkStatusCallback;
}

void RpcServer::SetGetIntermediateFileCallback(std::function<std::vector<std::string>(std::string)> GetIntermediateFileCallback)
{
    GetIntermediateFileCallback_ = GetIntermediateFileCallback;
}