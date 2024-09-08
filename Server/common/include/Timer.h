#pragma once

#include <functional>
#include <set>
#include <time.h>
#include <vector>
#include <mutex>

using Callback = std::function<void()>;

struct TimerNode
{
    time_t expired_; // 过期的timestamp
    time_t duration_; // 定时的时间
    uint id_; // expired_相同时使用id_判断 (会不会有重复id_)
    Callback func_; // 回调函数
    bool keepRunning_; // 是否循环定时
    TimerNode(time_t expired, time_t duration, uint id, Callback func, bool keepRunning): 
    expired_(expired), duration_(duration), id_(id), func_(func), keepRunning_(keepRunning) {}
    bool operator<(const TimerNode& timeNode) const
    {
        if(expired_ < timeNode.expired_) return true;
        else if(expired_ > timeNode.expired_) return false;
        else return id_ < timeNode.id_;
    }
};

class Timer
{
private:
    uint id_ = 0;
    std::set<TimerNode> timerSet_;
    time_t GetTick();
    void DeleteTimer(const TimerNode& timerNode);
public:
    void AddTimer(time_t duration, Callback callBack, bool keepRunning);
    void WaitExpired();
    void RunCallback();
};