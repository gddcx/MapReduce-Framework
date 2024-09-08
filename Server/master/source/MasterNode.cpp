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

void MasterNode::AddJob(std::string& filename)
{
    rpcServer_->AddJob(filename);
}

void MasterNode::StartMasterNode()
{
    rpcServer_->RunRpcServer();
}