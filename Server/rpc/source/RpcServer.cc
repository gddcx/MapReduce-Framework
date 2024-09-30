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
    uint taskId;
    uint jobId;
    Id id;
    uint jobNum = 0;
    if(GetReduceJobCallback_(nodeMsg->nodename(), key, value, taskId, jobId) == MR_OK)
    {
        jobMsg->set_key(key);
        jobMsg->set_value(value);
        jobMsg->set_type(masterSlaveRPC::TaskType::map);
        id.set_taskid(taskId);
        id.set_jobid(jobId);
        jobMsg->set_allocated_id(&id);
        return Status::OK;
    }
    else if(GetMapJobCallback_(nodeMsg->nodename(), key, value, taskId, jobId, jobNum) == MR_OK)
    {
        jobMsg->set_key(key);
        jobMsg->set_value(value);
        jobMsg->set_type(masterSlaveRPC::TaskType::map);
        id.set_taskid(taskId);
        id.set_jobid(jobId);
        jobMsg->set_allocated_id(&id);
        jobMsg->set_jobnum(jobNum);
        return Status::OK;
    }
    else
    {
        
    }

    return Status(StatusCode::UNAVAILABLE, "no available job!");
}

Status RpcServer::ReportJobStatus(ServerContext* context, const JobMessage* jobMsg, Empty* response)
{
    ChangeWorkStatusCallback_(jobMsg->type(), jobMsg->id().taskid(), jobMsg->id().jobid());
    return Status::OK;
}

Status RpcServer::HeartBeat(ServerContext* context, const NodeMessage* nodeMsg, Empty* response)
{
    std::string nodeName = nodeMsg->nodename();
    HeartBeatCallback_(nodeName);

    return Status::OK;
}

void RpcServer::SetGetMapJobCallback(std::function<bool(std::string, std::string&, std::string&, uint&, uint&, uint&)> GetMapJobCallback)
{
    GetMapJobCallback_ = GetMapJobCallback;
}

void RpcServer::SetGetReduceJobCallback(std::function<bool(std::string, std::string&, std::string&, uint&, uint&)> GetReduceJobCallback)
{
    GetReduceJobCallback_ = GetReduceJobCallback;
}

void RpcServer::SetChangeJobStatusCallback(std::function<void(int, uint, uint)> ChangeWorkStatusCallback)
{
    ChangeWorkStatusCallback_ = ChangeWorkStatusCallback;
}

void RpcServer::SetGetIntermediateFileCallback(std::function<std::vector<std::string>(std::string)> GetIntermediateFileCallback)
{
    GetIntermediateFileCallback_ = GetIntermediateFileCallback;
}

void RpcServer::SetHeartBeatCallback(std::function<void(std::string&)> HeartBeatCallback)
{
    HeartBeatCallback_ = HeartBeatCallback;
}