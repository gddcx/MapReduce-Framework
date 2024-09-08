#pragma once
#include <stack>
#include <string>
#include "RpcServer.h"

class MasterNode
{
private:
    RpcServer *rpcServer_;
public:
    MasterNode(std::string rpcListen);
    ~MasterNode();
    void AddJob(std::string& filename);
    void StartMasterNode();
};