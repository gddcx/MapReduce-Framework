#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "RpcServer.h"
#include "NodeManager.h"
#include "JobManager.h"

class MasterNode
{
private:
    RpcServer *rpcServer_ = nullptr;
    NodeManager nodeManager_;
    JobManager jobManager_;
    void StartJobManager();
    void StartNodeManager();
public:
    MasterNode(std::string rpcListen);
    ~MasterNode();
    void AddJob(std::vector<std::string>& keys, std::vector<std::string>& values, int reduceJobNum);
    void ChangeJobStatus(int jobType, uint taskId, uint jobId);
    bool GetMapJob(std::string nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& reduceJobNum);
    bool GetReduceJob(std::string nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& mapJobNum);
    void HeartBeatDetect(std::string& nodeName);
    void StartMasterNode();
};