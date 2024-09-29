#include "NodeManager.h"

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

void NodeManager::CheckNodeStatus()
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
            node->second.status_ = OFFLINE;
        }
    }
}

void NodeManager::NmStart()
{
    int beatCheckInterval = 30;  // TODO:worker必须30s内上报心跳，否则被认为丢失心跳
    timer_.AddTimer(beatCheckInterval, std::bind(&NodeManager::CheckNodeStatus, this), true);
}