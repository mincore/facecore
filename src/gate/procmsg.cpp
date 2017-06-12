/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: procmsg.cpp
 *     Created: 2017-05-31 11:49
 * Description:
 * ===================================================
 */
#include "common/common.h"
#include "module/module.h"
#include "server.h"

static bool Init()
{
    return ServerStart() == 0;
}

static bool Exit()
{
    return ServerStop() == 0;
}

MODULE(GATE, Init, Exit);

