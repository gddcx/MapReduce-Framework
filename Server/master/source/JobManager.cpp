#include <iostream>
#include "JobManager.h"

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

    tasks_.emplace(std::make_pair(taskId_, taskDesp));

    return taskId_++;
}

bool JobManager::JmAllocMapJob(std::string& key, std::string& value, std::string& nodeName, uint& taskId, uint& jobId)
{
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
                    taskIt->second.noStartMapJobNum_--;
                    return MR_OK;
                }
            }
        }
    }
    return MR_ERROR;
}

bool JobManager::JmAllocReduceJob(std::string& key, std::string& value, std::string& nodeName, uint& taskId, uint& jobId)
{
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
                    taskId = jobDesp->first;
                    jobId = jobDesp->first;
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
    const auto& task = tasks_.find(taskId);
    if((task != tasks_.end()) && (JobType == MAP_JOB))
    {
        auto& mapJob = task->second.mapJob_;
        const auto& job = mapJob.find(jobId);
        if(job != mapJob.end())
        {
            job->second.status_ = JOB_FINISHED;
        }
    }
}