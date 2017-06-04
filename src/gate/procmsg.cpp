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
#include "msg/msg.h"

static bool Init()
{
    return true;
}

static bool Exit()
{
    return true;
}

MODULE(GATE, Init, Exit);

