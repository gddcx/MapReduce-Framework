#pragma once
#include <vector>
#include <string>
#include "RpcServer.h"

#define JOB_NO_START        0
#define JOB_RUNNNING        1
#define JOB_FINISHED        2

struct WorkStatus
{
    std::string key_; // 需要处理的key
    std::string value_; // 需要处理的value
    int status_; // 完成状态, 0未开始，1进行中，2已完成
    std::string workerNodeName_; // 执行任务的节点名称
    WorkStatus(std::string key, std::string value, int status): key_(key), value_(value), status_(status) {}
    WorkStatus(std::string key, std::string value, int status, std::string nodeName): 
                                    key_(key), value_(value), status_(status), workerNodeName_(nodeName) {}
};

class MasterNode
{
private:
    RpcServer *rpcServer_;
    std::vector<WorkStatus> jobs_;
    std::vector<WorkStatus> reduceJobs_;
    std::mutex jobMutex_;
    int mapJobsNum_;
    int reduceJobsNum_;
public:
    MasterNode(std::string rpcListen);
    ~MasterNode();
    void SetReduceNodeNum(int num);
    int GetReduceNodeNum();
    void StartMasterNode();
    void AddJob(std::string& key, std::string& value);
    bool GetJob(std::string nodeName, std::string& key, std::string& value);
    void ChangeWorkStatus(std::string& nodeName);
    std::vector<std::string> GetIntermediateFile(std::string nodeName);
};