/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: module/module.cpp
 * Description:
 * =====================================================================
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "common/common.h"
#include "module.h"
#include "test/test.h"

extern Module *MODULE_ALG;
extern Module *MODULE_STORE;
extern Module *MODULE_GATE;

Module* gModules[] =
{
    MODULE_ALG,
    MODULE_STORE,
    MODULE_GATE,
};

static void ProcessEvents()
{
    run_tests();
}

int RunAllModule()
{
    for (size_t i=0; i<ARRAY_SIZE(gModules); i++)
    {
        Module *pModule = gModules[i];
        if (!pModule->Init())
        {
            LOG_ERROR("Init Module: %s failed", pModule->Name());
            return -1;
        }
        LOG_INFO("Module: %s Inited", pModule->Name());
    }

    ProcessEvents();

    for (size_t i=0; i<ARRAY_SIZE(gModules); i++)
    {
        Module *pModule = gModules[i];
        if (!pModule->Exit())
        {
            LOG_ERROR("Init Module: %s failed", pModule->Name());
            break;
        }
        LOG_INFO("Module: %s Exit", pModule->Name());
    }

    return 0;
}
