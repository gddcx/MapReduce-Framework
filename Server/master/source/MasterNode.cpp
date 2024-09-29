#include <functional>
#include <thread>
#include "MasterNode.h"

MasterNode::MasterNode(std::string rpcListen)
{
    rpcServer_ = new RpcServer(rpcListen);
}

MasterNode::~MasterNode()
{
    if(rpcServer_) delete rpcServer_;
}

void MasterNode::AddJob(std::vector<std::string>& keys, std::vector<std::string>& values, int reduceJobNum)
{
    jobManager_.JmAddNewTask(keys, values, reduceJobNum);
}


void MasterNode::ChangeJobStatus(int jobType, uint taskId, uint jobId)
{
    jobManager_.JmChangeJobStatus(JobType(jobType), taskId, jobId);
}

bool MasterNode::GetMapJob(std::string nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId)
{
    if(jobManager_.JmAllocMapJob(key, value, nodeName, taskId, jobId) == MR_OK)
    {
        nodeManager_.NmMonitorNewNode(0x7f000001, nodeName); // TODO：临时写入地址127.0.0.1
        return MR_OK;
    }
    return MR_ERROR;
}

bool MasterNode::GetReduceJob(std::string nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId)
{
    if(jobManager_.JmAllocReduceJob(key, value, nodeName, taskId, jobId) == MR_OK)
    {
        nodeManager_.NmMonitorNewNode(0x7f000001, nodeName); // TODO：临时写入地址127.0.0.1
        return MR_OK;
    }
    return MR_ERROR;
}

void MasterNode::HeartBeatDetect(std::string& nodeName)
{
    nodeManager_.NmSetNodeStatus(nodeName, ONLINE);
}

void MasterNode::StartMasterNode()
{
    rpcServer_->SetChangeJobStatusCallback(std::bind(&MasterNode::ChangeJobStatus, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    rpcServer_->SetGetMapJobCallback(std::bind(&MasterNode::GetMapJob, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    rpcServer_->SetGetReduceJobCallback(std::bind(&MasterNode::GetReduceJob, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    rpcServer_->SetHeartBeatCallback(std::bind(&MasterNode::HeartBeatDetect, this, std::placeholders::_1)); /* 心跳检测回调实现 */
    rpcServer_->RunRpcServer();
}