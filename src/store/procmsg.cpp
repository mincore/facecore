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
#include "store.h"
#include "alg/alg.h"

static bool Init()
{
    return Store::Instance()->Init();
}

static bool Exit()
{
    return Store::Instance()->Release();
}

MODULE(STORE, Init, Exit);

void MsgStoreAddFaces::Run()
{
    //Store::Instance()->AddFaces();
}

void MsgStoreListTags::Run()
{
    //Store::Instance()->ListTags();
}

void MsgSearchTagReq::Run()
{
    Store::Instance()->SearchTag(faceID, tagName);
}