/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: taskq.h
 *     Created: 2017-05-31 19:10
 * Description:
 * ===================================================
 */
#ifndef _WORKER_H
#define _WORKER_H

struct Task {
    virtual ~Task(){}
    virtual void Run() = 0;
};

class TaskQueueImpl;
class TaskQueue {
public:
    TaskQueue(int thread);
    ~TaskQueue();
    void Push(Task *task, int delay = 0);

private:
    TaskQueueImpl *m_impl;
};

#endif
