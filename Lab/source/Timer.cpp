#include <iostream>
#include <chrono>
#include <thread>
#include "Timer.h"

time_t Timer::GetTick()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

void Timer::AddTimer(time_t duration, Callback callback, bool keepRunning)
{
    time_t curTick = GetTick();
    timerSet_.emplace(curTick + duration, duration, callback, keepRunning);
}

void Timer::DeleteTimer(const TimerNode& timerNode)
{
    std::set<TimerNode>::const_iterator it = timerSet_.find(timerNode);
    timerSet_.erase(it);
}

void Timer::WaitExpired()
{
    time_t targetTick = timerSet_.begin()->expired_;
    time_t curTick = GetTick();
    if(targetTick > curTick)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(targetTick - curTick));
    }
}

void Timer::RunCallback()
{
    /* TODO:逻辑有问题，TimerNode的expired不会改变的，心跳检测一直新增/删除，那么就无法走进请求任务 */
    const TimerNode& timerNode = *timerSet_.begin();
    timerNode.func_();
    if(timerNode.keepRunning_)
    {
        AddTimer(timerNode.duration_, timerNode.func_, timerNode.keepRunning_);
    }
    DeleteTimer(timerNode);
}