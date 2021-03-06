/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: taskq.cpp
 *     Created: 2017-05-31 19:29
 * Description:
 * ===================================================
 */
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <list>
#include <vector>
#include <atomic>
#include "taskq.h"

typedef std::uint64_t Time;

static Time Now()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
            ).count();
}

class TaskQueueImpl {
public:
    TaskQueueImpl(int thread);
    ~TaskQueueImpl();

    void Push(Task *task, int delay);

private:
    void ThreadFunc();

    bool RunTask();
    bool RunDelayTask();
    void WaitForTask();
    bool HasTask();

private:
    std::atomic<bool> m_quit;
    std::vector<std::thread> m_threads;
    std::condition_variable m_taskCond;

    std::mutex m_taskMutex;
    std::list<Task*> m_taskList;
    std::multimap<Time, Task*> m_delayMap;
};

TaskQueueImpl::TaskQueueImpl(int thread): m_quit(false)
{
    for (int i=0; i<thread; i++) {
        m_threads.push_back(std::thread(&TaskQueueImpl::ThreadFunc, this));
    }
}

TaskQueueImpl::~TaskQueueImpl()
{
    m_quit = true;
    m_taskCond.notify_all();

    for (auto &thread : m_threads) {
        thread.join();
    }

    for (auto &task: m_taskList) {
        delete task;
    }

    for (auto &item: m_delayMap) {
        delete item.second;
    }
}

void TaskQueueImpl::Push(Task *task, int delay)
{
    std::unique_lock<std::mutex> lk(m_taskMutex);

    if (delay > 0) {
        Time t = Now() + delay;
        m_delayMap.insert(std::make_pair(t, task));
    }
    else
        m_taskList.push_back(task);

    m_taskCond.notify_one();
}

bool TaskQueueImpl::RunDelayTask()
{
    bool more = false;
    Task *task = NULL;
    {
        std::unique_lock<std::mutex> lk(m_taskMutex);
        if (!m_delayMap.empty()) {
            auto begin = m_delayMap.begin();
            if (begin->first <= Now()) {
                task = begin->second;
                m_delayMap.erase(begin);
                if (!m_delayMap.empty()) {
                    begin = m_delayMap.begin();
                    more = begin->first <= Now();
                }
            }
        }
    }

    if (task) {
        task->Run();
        delete task;
    }

    return more;
}

bool TaskQueueImpl::RunTask()
{
    bool more = false;
    Task *task = NULL;
    {
        std::unique_lock<std::mutex> lk(m_taskMutex);
        if (!m_taskList.empty()) {
            task = m_taskList.front();
            m_taskList.pop_front();
            more = !m_taskList.empty();
        }
    }

    if (task) {
        task->Run();
        delete task;
    }

    return more;
}

bool TaskQueueImpl::HasTask()
{
    if (m_quit)
        return true;

    if (!m_taskList.empty())
        return true;

    if (!m_delayMap.empty()) {
        if (m_delayMap.begin()->first <= Now())
            return true;
    }

    return false;
}

void TaskQueueImpl::WaitForTask()
{
    std::unique_lock<std::mutex> lk(m_taskMutex);
    if (HasTask()) {
        return;
    }

    auto begin = m_delayMap.begin();
    auto now = Now();
    if (begin->first > now) {
        m_taskCond.wait_for(lk, std::chrono::milliseconds(begin->first - now),
                [this]{ return HasTask(); });
    } else
        m_taskCond.wait(lk, [this]{ return HasTask(); });
}

void TaskQueueImpl::ThreadFunc()
{
    int more = 0;
    while (!m_quit) {
        more = RunTask();
        more |= RunDelayTask();
        if (!more)
            WaitForTask();
    }
}

TaskQueue::TaskQueue(int thread):m_impl(new TaskQueueImpl(thread))
{
}

TaskQueue::~TaskQueue()
{
    delete m_impl;
}

void TaskQueue::Push(Task *task, int delay)
{
    m_impl->Push(task, delay);
}
