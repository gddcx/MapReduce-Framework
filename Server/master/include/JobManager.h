#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "public.h"

#define JOB_NO_START        0
#define JOB_RUNNNING        1
#define JOB_FINISHED        2
#define JOB_WAITING         3 // 只给Reduce初始化用。目前策略是等待所有Map完成后才开始Reduce，Map全部完成后WAITTING->NO_START

enum JobType{
    MAP_JOB = 0,
    REDUCE_JOB,
};

struct JobDesp
{
    uint jobId_ = 0;
    std::string key_;            // 需要处理的key
    std::string value_;          // 需要处理的value
    std::string workerNodeName_; // 执行任务的节点名称
    int status_ = JOB_NO_START;  // 完成状态, 0未开始，1进行中，2已完成
    JobDesp(std::string key, std::string value): key_(key), value_(value) {}
    JobDesp(std::string key, std::string value, int status): key_(key), value_(value), status_(status) {}
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
    uint taskId_;
    std::unordered_map<uint, TaskDesp> tasks_;
public:
    uint JmAddNewTask(std::vector<std::string>& keys, std::vector<std::string>& values, int reduceJobNum);
    bool JmAllocMapJob(std::string& nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& reduceJobNum);
    bool JmAllocReduceJob(std::string& nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId);
    void JmChangeJobStatus(JobType JobType, uint taskId, uint jobId);
};