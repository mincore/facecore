/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: msg/msg.h
 * Description:
 * =====================================================================
 */
#ifndef MSG_H
#define MSG_H

#include "module/module.h"
#include "alg/alg.h"
#include "store/store.h"
#include "pb/faapi.pb.h"

struct MsgDetectReq: public Task, public faapi::DetectReq {
    virtual void Run();
};

struct MsgDetectResp: public Task, public faapi::DetectResp {
    virtual void Run();
};

struct MsgCheckinReq: public Task, public faapi::CheckinReq {
    virtual void Run();
};

struct MsgCheckinResp: public Task, public faapi::CheckinResp {
    virtual void Run();
};

struct MsgSearchReq: public Task, public faapi::SearchReq {
    virtual void Run();
};

struct MsgSearchResp: public Task, public faapi::SearchResp {
    virtual void Run();
};

#endif
