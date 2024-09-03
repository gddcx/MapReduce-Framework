#include "RpcClient.h"
#include "public.h"

MapReduceClient::MapReduceClient(std::string& target)
{
    stub_ = RequireJob::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
}

bool MapReduceClient::Require(bool isBusy, MapReduce::TaskType& taskType, std::string& filename)
{
    ClientContext context;
    NodeMessage nodeMsg;
    Reply reply;

    nodeMsg.set_isbusy(isBusy);
    Status status = stub_->Require(&context, nodeMsg, &reply);
    if(status.ok())
    {
        taskType = reply.tasktype();
        filename = reply.filename();
        return MR_OK;
    }
    return MR_ERROR;
}

void MapReduceClient::Finish(TaskType& taskType, std::string& filename)
{
    ClientContext context;
    FinishMsg finishMsg;
    google::protobuf::Empty res;

    finishMsg.set_filename(filename);
    finishMsg.set_tasktype(taskType);
    Status status = stub_->Finish(&context, finishMsg, &res);
    if(!status.ok())
    {
        std::cout << "Fail to report finish status" << std::endl;
    }
}