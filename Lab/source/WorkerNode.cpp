#include <thread>
#include <set>
#include "WorkerNode.h"
#include "RpcClient.h"
#include "Timer.h"
#include "public.h"
#include "function.h"

WorkerNode::~WorkerNode()
{
    delete rpcClient_;
}

void WorkerNode::ExecuteJob(TaskType& taskType, std::string& filename)
{
    bool retVal = MR_ERROR;

    switch(taskType)
    {
        case TaskType::map:
        {
            retVal = Map(filename);
            break;
        }
        case TaskType::reduce:
        {
            retVal = Reduce(filename);
            break;
        }
        default:
            break;
    }
    if(retVal == MR_OK) rpcClient_->Finish(taskType, filename);
}

void WorkerNode::CreateRpcClient(std::string& target)
{
    rpcClient_ = new MapReduceClient(target);
}

void WorkerNode::RequireJob()
{
    TaskType taskType;
    std::string filename;
    if(rpcClient_->Require(NON_BUSY, taskType, filename) == MR_OK)
    {
        std::thread jobThread(&WorkerNode::ExecuteJob, this, std::ref(taskType), std::ref(filename));
        jobThread.detach();
    }
}

void WorkerNode::HeartBeat() {}

void WorkerNode::StartWorkerNode()
{
    Timer timer;
    timer.AddTimer(5000, std::bind(&WorkerNode::HeartBeat, this), true); /* 5000ms上报心跳 */
    timer.AddTimer(1000, std::bind(&WorkerNode::RequireJob, this), true); /* busy不要请求任务 */
    for(;;)
    {
        timer.WaitExpired();
        timer.RunCallback();
    }
}