#include "RpcClient.h"
#include "public.h"

RpcClient::RpcClient(std::string& target)
{
    stub_ = JobRpc::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
}

bool RpcClient::Require(NodeMessage& nodeMsg, JobMessage& jobMsg)
{
    ClientContext context;

    Status status = stub_->RequireJob(&context, nodeMsg, &jobMsg);
    if(status.ok())
    {
        return MR_OK;
    }
    return MR_ERROR;
}

void RpcClient::JobFinished(JobMessage& jobMsg)
{
    ClientContext context;
    google::protobuf::Empty res;

    Status status = stub_->ReportJobStatus(&context, jobMsg, &res);
    if(!status.ok())
    {
        std::cout << "Fail to report finish status" << std::endl;
    }
}

void RpcClient::HeartBeat(NodeMessage& nodeMsg)
{
    ClientContext context;
    google::protobuf::Empty res;

    Status status = stub_->HeartBeat(&context, nodeMsg, &res);
}