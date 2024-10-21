#pragma once
#include <string>
#include "RpcClient.h"
#include "MapReduceBase.h"

#define INTERMEDIATE_FOLDER   "/home/dengchangxing/projects/MIT6824/Test/intermediate/"

class WorkerNode
{
private:
    std::string nodeName_ = "";
    std::string target_ = "";
    RpcClient* rpcClient_ = nullptr;
    void* handle_ = nullptr;
    MapReduceBase* mrObj_ = nullptr;
    MapReduceBase* (*CreateMapReduceInstance_)() = nullptr;
    void (*DestroyMapReduceInstance_)(MapReduceBase*) = nullptr;
public:
    void SetNodeName(std::string nodeName);
    int LoadCustomizedMapReduce(const std::string& libPath);

private:
    void ExecuteJob(JobMessage jobMsg);
    void Partition(std::vector<std::pair<std::string, int>>& mapRes, uint partNum, uint taskId, uint jobId);
    void LoadPartition(const std::string& filename, std::vector<std::pair<std::string, int>>& partition);
    void FetchIntermediaData(uint& taskId, uint& jobNum, std::string& filename, std::vector<std::string>& filenameVec);
    void Merge(std::vector<std::pair<std::string, int>>& partition, std::unordered_map<std::string, int>& res);
public:
    ~WorkerNode();
    void SetRpcServer(std::string target);
    void RequireJob();
    void HeartBeat();
    void StartWorkerNode(std::string libPath);
};