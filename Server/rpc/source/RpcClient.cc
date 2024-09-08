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

void RpcClient::JobFinished(NodeMessage& nodeMsg)
{
    ClientContext context;
    google::protobuf::Empty res;

    Status status = stub_->ReportJobStatus(&context, nodeMsg, &res);
    if(!status.ok())
    {
        std::cout << "Fail to report finish status" << std::endl;
    }
}

bool RpcClient::Fetch(NodeMessage& nodeMsg, MapDataList& mapDataList)
{
    ClientContext context;

    Status status = stub_->FetchDataFromMap(&context, nodeMsg, &mapDataList);
    if(status.ok())
    {
        return MR_OK;
    }
    return MR_ERROR;
}