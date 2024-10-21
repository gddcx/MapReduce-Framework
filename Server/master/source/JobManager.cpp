#include <iostream>
#include "JobManager.h"
#include "Timer.h"

void JobManager::JmSetNodeManager(NodeManager* nodeManager)
{
    nodeManager_ = nodeManager;
}

uint JobManager::JmAddNewTask(std::vector<std::string>& keys, std::vector<std::string>& values, int reduceJobNum)
{
    if(keys.size() != values.size())
    {
        std::cout << __func__ << ":" << "size of keys(" << keys.size() << ") not equal to size of values(" << values.size() << ")" << std::endl;
        return -1;
    }
    
    TaskDesp taskDesp;
    for(int loop = 0; loop < keys.size(); loop++)
    {
        taskDesp.mapJob_.emplace(std::make_pair(taskDesp.jobCnt_, JobDesp(keys[loop], values[loop])));
        taskDesp.jobCnt_++;
    }
    taskDesp.noStartMapJobNum_ = taskDesp.mapJob_.size();

    std::string key = "";
    for(int loop = 0; loop < reduceJobNum; loop++)
    {
        key = INTERMEDIATE_FILE_PREFIX + std::to_string(loop) + INTERMEDIATE_FILE_SUBFIX;
        taskDesp.reduceJob_.emplace(std::make_pair(taskDesp.jobCnt_, JobDesp(key, "", JOB_WAITING)));
        taskDesp.jobCnt_++;
    }
    taskDesp.noStartReduceJobNum_ = taskDesp.reduceJob_.size();

    std::lock_guard<std::mutex> lock(taskMutex_);
    tasks_.emplace(std::make_pair(taskId_, taskDesp));

    return taskId_++;
}

bool JobManager::JmAllocMapJob(std::string& nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& reduceJobNum)
{
    std::lock_guard<std::mutex> lock(taskMutex_);

    auto taskIt = tasks_.begin();
    for(; taskIt != tasks_.end(); taskIt++)
    {
        if(taskIt->second.noStartMapJobNum_ != 0)
        {
            auto& mapJobDesps = taskIt->second.mapJob_;
            for(auto jobDesp = mapJobDesps.begin(); jobDesp != mapJobDesps.end(); jobDesp++)
            {
                if(jobDesp->second.status_ == JOB_NO_START)
                {
                    jobDesp->second.status_ = JOB_RUNNNING;
                    jobDesp->second.workerNodeName_ = nodeName;
                    key = jobDesp->second.key_;
                    value = jobDesp->second.value_;
                    taskId = taskIt->first;
                    jobId = jobDesp->first;
                    reduceJobNum = taskIt->second.reduceJob_.size();
                    taskIt->second.noStartMapJobNum_--;
                    return MR_OK;
                }
            }
        }
    }
    return MR_ERROR;
}

bool JobManager::JmAllocReduceJob(std::string& nodeName, std::string& key, std::string& value, uint& taskId, uint& jobId, uint& mapJobNum)
{
    std::lock_guard<std::mutex> lock(taskMutex_);

    auto taskIt = tasks_.begin();
    for(; taskIt != tasks_.end(); taskIt++)
    {
        if(taskIt->second.noStartReduceJobNum_ != 0)
        {
            auto& reduceJobDesps = taskIt->second.reduceJob_;
            for(auto jobDesp = reduceJobDesps.begin(); jobDesp != reduceJobDesps.end(); jobDesp++)
            {
                if(jobDesp->second.status_ == JOB_NO_START)
                {
                    jobDesp->second.status_ = JOB_RUNNNING;
                    jobDesp->second.workerNodeName_ = nodeName;
                    key = jobDesp->second.key_;
                    value = jobDesp->second.value_;
                    taskId = taskIt->first;
                    jobId = jobDesp->first;
                    mapJobNum = taskIt->second.mapJob_.size();
                    taskIt->second.noStartReduceJobNum_--;
                    return MR_OK;
                }
            }
        }
    }
    return MR_ERROR;
}

void JobManager::JmChangeJobStatus(JobType JobType, uint taskId, uint jobId)
{
    std::lock_guard<std::mutex> lock(taskMutex_);

    const auto& task = tasks_.find(taskId);
    if(task != tasks_.end())
    {
        if(JobType == MAP_JOB)
        {
            auto& mapJob = task->second.mapJob_;
            const auto& job = mapJob.find(jobId);
            if(job != mapJob.end())
            {
                task->second.finishedMapJobNum_++;
                job->second.status_ = JOB_FINISHED;
            }
            if(task->second.finishedMapJobNum_ == mapJob.size())
            {
                auto& reduceJob = task->second.reduceJob_;
                auto reduceIt = reduceJob.begin();
                while(reduceIt != reduceJob.end())
                {
                    reduceIt->second.status_ = JOB_NO_START;
                    reduceIt++;
                }
            }
        }
        else if(JobType == REDUCE_JOB)
        {
            auto& reduceJob = task->second.reduceJob_;
            const auto& job = reduceJob.find(jobId);
            if(job != reduceJob.end())
            {
                task->second.finishedReduceJobNum_++;
                job->second.status_ = JOB_FINISHED;
                task->second.reduceJob_.erase(job);
            }
            if(task->second.finishedReduceJobNum_ == reduceJob.size())
            {
                tasks_.erase(task);
            }
        }
    }
}

void JobManager::JmCheckDeadTask()
{
    std::lock_guard<std::mutex> lock(taskMutex_);

    uint taskId = 0;
    for(auto& task: tasks_)
    {
        taskId = task.first;
        TaskDesp& taskdesp = task.second;
        for(auto& mpJob: taskdesp.mapJob_)
        {
            if((!mpJob.second.workerNodeName_.empty()) && (nodeManager_->NmGetNodeStatus(mpJob.second.workerNodeName_) == OFFLINE))
            {
                mpJob.second.status_ = JOB_NO_START;
                task.second.noStartMapJobNum_++;
            }
        }
        for(auto& reduceJob: taskdesp.reduceJob_)
        {
            if((!reduceJob.second.workerNodeName_.empty()) && (nodeManager_->NmGetNodeStatus(reduceJob.second.workerNodeName_) == OFFLINE))
            {
                reduceJob.second.status_ = JOB_NO_START;
                task.second.noStartReduceJobNum_++;
            }
        }
    }
}

void JobManager::JmMonitorStart()
{
    Timer timer;
    int interval = 10000;
    timer.AddTimer(interval, std::bind(&JobManager::JmCheckDeadTask, this), true);

    for(;;)
    {
        timer.WaitExpired();
        timer.RunCallback();
    }
}