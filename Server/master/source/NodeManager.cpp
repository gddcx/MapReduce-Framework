#include "NodeManager.h"
#include <iostream>

void NodeManager::NmMonitorNewNode(uint addr, std::string nodeName)
{
    if(nodes_.find(nodeName) == nodes_.end())
    {
        nodes_.emplace(nodeName, Node(addr, nodeName));
    }
}

int NodeManager::NmGetNodeStatus(std::string nodeName)
{
    const auto& it = nodes_.find(nodeName);
    if(it == nodes_.end())
    {
        return OFFLINE;
    }
    else
    {
        return it->second.status_;
    }
}

void NodeManager::NmSetNodeStatus(std::string nodeName, int status)
{
    const auto& it = nodes_.find(nodeName);
    if(it != nodes_.end())
    {
        it->second.status_ = status;
        it->second.updateFlag_ = 1;
    }
}

void NodeManager::NmCheckNodeStatus()
{
    auto node = nodes_.begin();
    for(; node != nodes_.end(); node++)
    {
        if(node->second.updateFlag_)
        {
            node->second.updateFlag_ = false;
        }
        else
        {
            std::cout << __func__ << " node:" << node->second.nodeName_ << " offline" << std::endl;
            node->second.status_ = OFFLINE;
        }
    }
}

void NodeManager::NmStart()
{
    int beatCheckInterval = 20000;  // TODO:worker必须20s内上报心跳，否则被认为丢失心跳
    timer_.AddTimer(beatCheckInterval, std::bind(&NodeManager::NmCheckNodeStatus, this), true);

    for(;;)
    {
        timer_.WaitExpired();
        timer_.RunCallback();
    }
}