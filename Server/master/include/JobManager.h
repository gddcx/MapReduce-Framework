#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include "public.h"
#include "NodeManager.h"

#define JOB_NO_START        0
#define JOB_RUNNNING        1
#define JOB_FINISHED        2
#define JOB_WAITING         3 // 只给Reduce初始化用。目前策略是等待所有Map完成后才开始Reduce，Map全部完成后WAITTING->NO_START

#define JOB_WAIT_TIME       10

enum JobType{
    MAP_JOB = 0,
    REDUCE_JOB,
};

struct JobBestNode
{
    std::string nodeName_;
    int requiedCnt_;
    JobBestNode() = default;
    JobBestNode(std::string& nodeName): nodeName_(nodeName), requiedCnt_(0) {}
};

struct JobDesp
{
    /* TODO:先临时假设存在这样的 文件->节点 的映射；之后改到从分布式文件系统获取*/
    std::unordered_map<std::string, std::string> _fileNodeMap = {
        {"pg-being_ernest.txt",     "node1"},
        {"pg-dorian_gray.txt",      "node2"},
        {"pg-frankenstein.txt",     "node3"},
        {"pg-grimm.txt",            "node1"},
        {"pg-huckleberry_finn.txt", "node2"},
        {"pg-metamorphosis.txt",    "node3"},
        {"pg-sherlock_holmes.txt",  "node1"},
        {"pg-tom_sawyer.txt",       "node2"}
    };
    uint jobId_ = 0;
    std::string key_;            // 需要处理的key
    std::string value_;          // 需要处理的value
    std::string workerNodeName_ = ""; // 执行任务的节点名称
    int status_ = JOB_NO_START;  // 完成状态, 0未开始，1进行中，2已完成
    JobBestNode jobBestNode_;
    JobDesp(std::string key, std::string value): key_(key), value_(value) {
        jobBestNode_ = JobBestNode(_fileNodeMap[key]); // TODO：从分布式文件系统获取文件到节点的映射
    }
    JobDesp(std::string key, std::string value, int status): key_(key), value_(value), status_(status) {
        jobBestNode_ = JobBestNode(_fileNodeMap[key]);
    }
};

/*
|-------------------------------|
|           TaskDesp            |
|-------------------------------|
|  mapJob_: JobDesp1,JobDesp2   |
|-------------------------------|
| reduceJob: JobDesp1,JobDescp2 |
|-------------------------------|
*/
struct TaskDesp
{
    uint jobCnt_ = 0;
    uint noStartMapJobNum_ = 0;
    uint finishedMapJobNum_ = 0;
    std::unordered_map<uint, JobDesp> mapJob_;
    uint noStartReduceJobNum_ = 0;
    uint finishedReduceJobNum_ = 0;
    std::unordered_map<uint, JobDesp> reduceJob_;
};

class JobManager
{
private:
    uint taskId_ = 0;
    std::unordered_map<uint, TaskDesp> tasks_;
    std::mutex taskMutex_;
    NodeManager* nodeManager_;
public:
    void JmSetNodeManager(NodeManager* nodeManager);
    uint JmAddNewTask(std::vector<std::string>& keys, std::vector<std::string>& values, int reduceJobNum);
    bool JmAllocMapJob(std::string& nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& reduceJobNum);
    bool JmAllocReduceJob(std::string& nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& mapJobNum);
    void JmChangeJobStatus(JobType JobType, uint taskId, uint jobId);
    void JmCheckDeadTask();
    void JmMonitorStart();
};