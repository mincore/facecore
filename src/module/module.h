/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: module/module.h
 * Description:
 * =====================================================================
 */
#ifndef _MODULE_H
#define _MODULE_H

#include <stdlib.h>
#include "common/common.h"
#include "taskq/taskq.h"

struct Module {
public:
    virtual bool Init() = 0;
    virtual bool Exit() = 0;
    void Push(Task *task, int delay = 0) {
        m_taskq->Push(task, delay);
    }
    const char* Name() { return m_name; }
protected:
    const char *m_name;
    TaskQueue *m_taskq;
};

#define MACROSTR(x) #x
#define MODULE(name, INITFUNC, EXITFUNC)\
    class Module##name: public Module {\
    public:\
        virtual bool Init() {\
            m_name = MACROSTR(MODULE_##name);\
            m_taskq = new TaskQueue(1);\
            return ::INITFUNC();\
        }\
        virtual bool Exit() {\
            delete m_taskq;\
            return ::EXITFUNC();\
        }\
    };\
    static Module##name gModule;\
    Module* MODULE_##name = &gModule;

int RunAllModule();

#endif
