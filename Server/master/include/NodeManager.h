#pragma once
#include "Timer.h"
#include <unordered_map>

#define OFFLINE     (0)
#define ONLINE      (1)

struct Node
{
    uint addr_;             // 节点地址
    std::string nodeName_;  // 节点名称
    int status_;            // 节点心跳状态：0-心跳丢失，1-心跳正常
    int cpuRate_;
    int memRate_;
    bool updateFlag_;       // 心跳更新标志: 0-心跳状态没有更新 1-心跳状态更新
    Node(uint addr, std::string nodeName): addr_(addr), nodeName_(nodeName) 
    {
        status_ = ONLINE;
        updateFlag_ = true;
    }
};

class NodeManager
{
private:
    Timer timer_;
    std::unordered_map<std::string, Node> nodes_;
public:
    void NmMonitorNewNode(uint addr, std::string nodeName);
    int NmGetNodeStatus(std::string nodeName);
    void NmSetNodeStatus(std::string nodeName, int status);
private:
    void CheckNodeStatus();
public:
    void NmStart();
};