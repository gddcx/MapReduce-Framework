#include <thread>
#include "MasterNode.h"

MasterNode::~MasterNode()
{
    delete rpcServer_;
}

void MasterNode::AddJob(std::string& filename)
{
    rpcServer_->AddJob(filename);
}

void MasterNode::StartMasterNode(std::string& rpcListen)
{
    rpcServer_ = new MapReduceServer(rpcListen);
    rpcServer_->RunRpcServer();
}