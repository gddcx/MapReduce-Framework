#include <functional>
#include <thread>
#include "MasterNode.h"

MasterNode::MasterNode(std::string rpcListen)
{
    rpcServer_ = new RpcServer(rpcListen);
}

MasterNode::~MasterNode()
{
    delete rpcServer_;
}

void MasterNode::SetReduceNodeNum(int num)
{   
    std::string key;
    reduceJobsNum_ = num;
    for(int idx = 0; idx < num; idx++)
    {
        key = INTERMEDIATE_FILE_PREFIX + std::to_string(idx) + INTERMEDIATE_FILE_SUBFIX;
        reduceJobs_.emplace_back(key, "", JOB_NO_START);
        key = "";
    }
}

int MasterNode::GetReduceNodeNum()
{
    return reduceJobsNum_ > 0 ? reduceJobsNum_-- : 0;
}

void MasterNode::StartMasterNode()
{
    rpcServer_->SetChangeWorkStatusCallback(std::bind(&MasterNode::ChangeWorkStatus, this, std::placeholders::_1));
    rpcServer_->SetGetIntermediateFileCallback(std::bind(&MasterNode::GetIntermediateFile, this, std::placeholders::_1));
    rpcServer_->SetGetMapJobCallback(std::bind(&MasterNode::GetJob, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    rpcServer_->SetGetReduceJobNumCallback(std::bind(&MasterNode::GetReduceNodeNum, this));
    rpcServer_->RunRpcServer();
}

void MasterNode::AddJob(std::string& key, std::string& value)
{
    std::lock_guard<std::mutex> lock(jobMutex_);

    jobs_.emplace_back(key, value, JOB_NO_START);
    mapJobsNum_ = jobs_.size();
}

bool MasterNode::GetJob(std::string nodeName, std::string& key, std::string& value) 
{
    std::lock_guard<std::mutex> lock(jobMutex_);

    for(auto& workStatus: jobs_)
    {
        if(workStatus.status_ == JOB_NO_START)
        {
            workStatus.status_ = JOB_RUNNNING;
            workStatus.workerNodeName_ = nodeName;
            key = workStatus.key_;
            value = workStatus.value_;
            return MR_OK;
        }
    }
    return MR_ERROR;
}

void MasterNode::ChangeWorkStatus(std::string& nodeName)
{
    for(auto& workStatus: jobs_)
    {
        if(workStatus.workerNodeName_ == nodeName)
        {
            workStatus.status_ = JOB_FINISHED;
            mapJobsNum_--;
            break;
        }
    }
}

std::vector<std::string> MasterNode::GetIntermediateFile(std::string nodeName)
{
    std::stringstream url;
    std::vector<std::string> res;
    WorkStatus* workStatus;

    for(size_t idx = 0; idx < reduceJobs_.size(); idx++)
    {
        workStatus = &reduceJobs_[idx];
        if(workStatus->status_ == JOB_NO_START)
        {
            workStatus->status_ = JOB_RUNNNING;
            workStatus->workerNodeName_ = nodeName;
            for(const auto& workStatus: jobs_)
            {
                url << workStatus.workerNodeName_ << "/" << INTERMEDIATE_FILE_PREFIX << idx << INTERMEDIATE_FILE_SUBFIX;
                res.push_back(url.str());
                url.str("");
            }
            break;
        }
    }

    return res;
}