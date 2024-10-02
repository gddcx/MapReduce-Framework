#include <thread>
#include <set>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>
#include <algorithm>
#include "WorkerNode.h"
#include "Timer.h"
#include "public.h"
#include "MapReduceBase.h"

WorkerNode::~WorkerNode()
{
    if(rpcClient_) delete rpcClient_;
    if(mrObj_) DestroyMapReduceInstance_(mrObj_);
    if(handle_) dlclose(handle_);
}

void WorkerNode::SetNodeName(std::string nodeName)
{
    nodeName_ = nodeName;
}

int WorkerNode::LoadCustomizedMapReduce(const std::string& libPath)
{
    handle_ = dlopen(libPath.c_str(), RTLD_LAZY);
    if(!handle_)
    {
        std::cout << "fail to load dll" << std::endl;
        return -1;
    }
    CreateMapReduceInstance_ = (MapReduceBase* (*)()) dlsym(handle_, "CreateMapReduceInstance");
    if(!CreateMapReduceInstance_)
    {
        std::cout << "fail to find function:CreateMapReduceInstance" << std::endl;
        return -1;
    }
    DestroyMapReduceInstance_ = (void (*)(MapReduceBase*)) dlsym(handle_, "DestroyMapReduceInstance");
    if(!DestroyMapReduceInstance_)
    {
        std::cout << "fail to find function:DestroyMapReduceInstance" << std::endl;
        return -1;
    }
    mrObj_ = CreateMapReduceInstance_();

    return 0;
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
        std::thread jobThread(&WorkerNode::ExecuteJob, this, jobsMsg);
        jobThread.detach();
    }
}

void WorkerNode::Merge(std::vector<std::pair<std::string, int>>& partition, std::unordered_map<std::string, int>& res)
{
    for(const auto& pair: partition)
    {
        res[pair.first] += pair.second;
    }
}

void WorkerNode::ExecuteJob(JobMessage jobMsg)
{
    std::vector<std::pair<std::string, int>> mapRes;
    TaskType taskType = jobMsg.type();
    std::string key = jobMsg.key();
    std::string value = jobMsg.value();
    const Id& id = jobMsg.id();
    uint taskId = id.taskid();
    uint jobId = id.jobid();
    uint jobNum = jobMsg.jobnum();
    switch(taskType)
    {
        case TaskType::map:
        {
            std::string cmd = "mkdir -p " + std::string(INTERMEDIATE_FOLDER) + std::to_string(taskId) + "/" + std::to_string(jobId);
            system(cmd.c_str());
            mapRes = mrObj_->Map(key, value);
            Partition(mapRes, jobNum, taskId, jobId);
            break;
        }
        case TaskType::reduce:
        {
            std::vector<std::pair<std::string, int>> partition;
            std::unordered_map<std::string, int> res;
            std::vector<std::string> filenameVec;
            std::vector<std::string> keys;
            std::vector<int> values;
            FetchIntermediaData(taskId, jobId, key, filenameVec);
            for(auto& filename: filenameVec)
            {
                LoadPartition(filename, partition);
                Merge(partition, res);
                std::vector<std::pair<std::string, int>>().swap(partition);
            }
            std::transform(res.begin(), res.end(), std::back_inserter(keys), [](const std::pair<std::string, int>& p){ return p.first; });
            std::transform(res.begin(), res.end(), std::back_inserter(values), [](const std::pair<std::string, int>& p) { return p.second; });
            mrObj_->Reduce(keys, values);
            break;
        }
        default:
            break;
    }
    rpcClient_->JobFinished(jobMsg);
}

void WorkerNode::Partition(std::vector<std::pair<std::string, int>>& mapRes, uint partNum, uint taskId, uint jobId)
{   
    std::vector<std::vector<std::pair<std::string, int>>> partitions(partNum);
    std::vector<std::pair<std::string, int>>::const_iterator it;
    std::hash<std::string> hash_fn;
    for(it = mapRes.begin(); it!=mapRes.end(); it++)
    {
        partitions[hash_fn((*it).first) % partNum].emplace_back(*it);
    }
    std::stringstream filename;
    std::ofstream fd;
    std::size_t size;
    std::size_t pairStrSize;
    std::string folder = INTERMEDIATE_FOLDER + std::to_string(taskId) + "/" + std::to_string(jobId) + "/";
    for(int reducerId = 0; reducerId < partNum; reducerId++)
    {   
        filename << folder << INTERMEDIATE_FILE_PREFIX << reducerId << INTERMEDIATE_FILE_SUBFIX;
        fd.open(filename.str(), std::ios::out | std::ios::binary);
        size = partitions[reducerId].size();
        if(size != 0)
        {
            fd.write(reinterpret_cast<const char*>(&size), sizeof(size));
            for(const auto& pair: partitions[reducerId])
            {
                pairStrSize = pair.first.size();
                fd.write(reinterpret_cast<const char*>(&pairStrSize), sizeof(pairStrSize));
                fd.write(pair.first.c_str(), pairStrSize);
                fd.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
            }
        }
        fd.close();
        filename.str("");
    }
}

void WorkerNode::LoadPartition(const std::string& filename, std::vector<std::pair<std::string, int>>& partition)
{
    std::ifstream fd;
    std::size_t size;
    std::size_t tmpStrSize;

    fd.open(filename, std::ios::in | std::ios::binary);
    fd.read(reinterpret_cast<char*>(&size), sizeof(size));
    partition.resize(size);
    for(auto& pair: partition)
    {
        fd.read(reinterpret_cast<char*>(&tmpStrSize), sizeof(tmpStrSize));
        pair.first.resize(tmpStrSize);
        fd.read(&pair.first[0], tmpStrSize);
        fd.read(reinterpret_cast<char*>(&pair.second), sizeof(pair.second));
    }
    fd.close();
}

void WorkerNode::FetchIntermediaData(uint& taskId, uint& jobId, std::string& filename, std::vector<std::string>& filenameVec)
{
    std::string path = INTERMEDIATE_FOLDER + std::to_string(taskId) + "/";
    for(int loop = 0; loop < jobId; loop++)
    {
        filenameVec.emplace_back(path + std::to_string(loop) + "/" + filename);
    }
}

void WorkerNode::HeartBeat()
{
    NodeMessage nodeMsg;
    nodeMsg.set_nodename(nodeName_);
    rpcClient_->HeartBeat(nodeMsg);
}

void WorkerNode::StartWorkerNode(std::string libPath)
{
    if(LoadCustomizedMapReduce(libPath) == -1)
    {
        std::cout << "not found MapReduce dll" << std::endl;
        return;
    }

    Timer timer;
    timer.AddTimer(5000, std::bind(&WorkerNode::HeartBeat, this), true); /* 5000ms上报心跳 */
    timer.AddTimer(2000, std::bind(&WorkerNode::RequireJob, this), true); /* busy不要请求任务 */
    for(;;)
    {
        timer.WaitExpired();
        timer.RunCallback();
    }
    
}