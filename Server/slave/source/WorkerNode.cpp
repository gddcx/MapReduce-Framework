#include <thread>
#include <set>
#include <fstream>
#include <iostream>
#include "WorkerNode.h"
#include "Timer.h"
#include "public.h"
#include "MapReduceBase.h"

WorkerNode::~WorkerNode()
{
    if(rpcClient_ != nullptr) delete rpcClient_;
}

void WorkerNode::SetNodeName(std::string nodeName)
{
    nodeName_ = nodeName;
}

void WorkerNode::CreateRpcClient(std::string& target)
{
    rpcClient_ = new RpcClient(target);
}

void WorkerNode::RequireJob()
{
    NodeMessage nodeMsg;
    JobMessage jobsMsg;
    nodeMsg.set_nodename(nodeName_);
    if(rpcClient_->Require(nodeMsg, jobsMsg) == MR_OK)
    {
        std::thread jobThread(&WorkerNode::ExecuteJob, this, std::ref(jobsMsg));
        jobThread.detach();
    }
}

void WorkerNode::ExecuteJob(JobMessage& jobMsg)
{
    std::vector<std::pair<std::string, int>> mapRes;
    JobMessage_TaskType taskType = jobMsg.type();
    std::string key = jobMsg.key();
    std::string value = jobMsg.value();
    switch(taskType)
    {
        /* TODO: mrObj_ 初始化*/
        case JobMessage_TaskType::JobMessage_TaskType_map:
        {
            mapRes = mrObj_->Map(key, value);
            Partition(mapRes);
            break;
        }
        case JobMessage_TaskType::JobMessage_TaskType_reduce:
        {
            MapDataList mapDataList;
            std::vector<std::pair<std::string, int>> partition;
            std::vector<std::pair<std::string, int>> mergePartitions;
            FetchIntermediaData(mapDataList);
            for(const auto& filename: mapDataList.filename())
            {
                LoadPartition(filename, partition);
                mergePartitions.insert(mergePartitions.end(), partition.begin(), partition.end());
                std::vector<std::pair<std::string, int>>().swap(partition);
            }
            mrObj_->Reduce(key, mergePartitions);
            break;
        }
        default:
            break;
    }
    // if(retVal == MR_OK)
    // {
    //     /* 先假设每次只有一个job，后面加job ID，用来创建Job的文件夹，同时上报完成的Job是哪一个 */
    //     NodeMessage nodeMsg;
    //     nodeMsg.set_nodename(nodeName_);
    //     rpcClient_->JobFinished(nodeMsg);
    // }
}

void WorkerNode::Partition(std::vector<std::pair<std::string, int>>& mapRes)
{   
    int reducerNum = 10;
    std::vector<std::vector<std::pair<std::string, int>>> partitions(10);
    std::vector<std::pair<std::string, int>>::const_iterator it = mapRes.begin();
    std::hash<std::string> hash_fn;
    for(; it!=mapRes.end(); it++)
    {
        partitions[hash_fn((*it).first) % reducerNum].emplace_back(*it);
    }

    std::stringstream filename;
    std::ofstream fd;
    std::size_t size;
    for(int reducerId = 0; reducerId < reducerNum; reducerId++)
    {
        filename << "intermediate-" << reducerId << ".bin";
        fd.open(filename.str(), std::ios::out | std::ios::binary);
        size = partitions[reducerId].size();
        fd.write(reinterpret_cast<const char*>(&size), sizeof(size));
        fd.write(reinterpret_cast<const char*>(partitions[reducerId].data()), size * sizeof(std::pair<std::string, int>));
        fd.close();
        filename.str("");
    }
}

void WorkerNode::LoadPartition(const std::string& filename, std::vector<std::pair<std::string, int>>& partition)
{
    std::ifstream fd;
    std::size_t size;
    fd.open(filename, std::ios::in | std::ios::binary);
    fd.read(reinterpret_cast<char*>(&size), sizeof(size));
    fd.read(reinterpret_cast<char*>(partition.data()), size * sizeof(std::pair<std::string, int>));
}

void WorkerNode::FetchIntermediaData(MapDataList& mapDataList)
{
    NodeMessage nodeMsg;
    nodeMsg.set_nodename(nodeName_);
    rpcClient_->Fetch(nodeMsg, mapDataList);
}

void WorkerNode::HeartBeat() {}

void WorkerNode::StartWorkerNode()
{
    Timer timer;
    timer.AddTimer(500, std::bind(&WorkerNode::HeartBeat, this), true); /* 5000ms上报心跳 */
    timer.AddTimer(1000, std::bind(&WorkerNode::RequireJob, this), true); /* busy不要请求任务 */
    for(;;)
    {
        timer.WaitExpired();
        timer.RunCallback();
    }
}