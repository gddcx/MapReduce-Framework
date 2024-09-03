#pragma once
#include <stack>
#include <string>
#include "RpcServer.h"
class MasterNode
{
private:
    MapReduceServer *rpcServer_;
public:
    ~MasterNode();
    void AddJob(std::string& filename);
    void StartMasterNode(std::string& rpcListen);
};