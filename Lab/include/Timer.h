#pragma once

#include <functional>
#include <set>
#include <time.h>
#include <vector>

using Callback = std::function<void()>;

struct TimerNode
{
    time_t expired_;
    time_t duration_;
    Callback func_;
    bool keepRunning_;
    TimerNode(time_t expired, time_t duration, Callback func, bool keepRunning): 
    expired_(expired), duration_(duration), func_(func), keepRunning_(keepRunning) {}
    bool operator<(const TimerNode& timeNode) const
    {
        if(expired_ < timeNode.expired_) return true;
        else return false;
    }
};

class Timer
{
private:
    std::set<TimerNode> timerSet_;
    time_t GetTick();
    void DeleteTimer(const TimerNode& timerNode);
public:
    void AddTimer(time_t duration, Callback callBack, bool keepRunning);
    void WaitExpired();
    void RunCallback();
};