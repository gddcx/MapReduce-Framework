#pragma once
#include <string>
#include "RpcClient.h"

class WorkerNode
{
private:
    MapReduceClient* rpcClient_;
    void ExecuteJob(TaskType& taskType, std::string& filename);
public:
    ~WorkerNode();
    void CreateRpcClient(std::string& target);
    void RequireJob();
    void HeartBeat();
    void StartWorkerNode();
};