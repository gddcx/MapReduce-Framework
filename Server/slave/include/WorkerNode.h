#pragma once
#include <string>
#include "RpcClient.h"
#include "MapReduceBase.h"

class WorkerNode
{
private:
    std::string nodeName_;
    RpcClient* rpcClient_;
    MapReduceBase* mrObj_;
public:
    void SetNodeName(std::string nodeName);

private:
    void ExecuteJob(JobMessage jobMsg);
    void Partition(std::vector<std::pair<std::string, int>>& mapRes);
    void LoadPartition(const std::string& filename, std::vector<std::pair<std::string, int>>& partition);
    void FetchIntermediaData(MapDataList& mapDataList);
public:
    ~WorkerNode();
    void CreateRpcClient(std::string& target);
    void RequireJob();
    void HeartBeat();
    void StartWorkerNode();
};