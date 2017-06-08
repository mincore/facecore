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
#include <vector>
#include <string>

struct Msg: public Task {
    int sess;
};

struct MsgDetectReq: public Msg {
    void Run();

    std::string imgFile;
};

struct MsgDetectResp: public Msg {
    void Run();

    struct result {
        int x,y,w,h;
    };
    std::vector<result> results;
};

struct MsgSearchReq: public Msg {
    void Run();

    std::string imgFile;
    std::vector<std::string> tagNames;
};

struct MsgSearchResp: public Msg {
    void Run();

    struct result {
        FaceID matchFaceID;
        float score;
        std::string tagName;
    };
    std::vector<result> results;
};

struct MsgAddImgReq: public Msg {
    void Run();

    std::vector<std::string> imgFiles;
    std::string tagName;
};

struct MsgAddImgResp: public Msg {
    void Run();

    int validImgs;
};

struct MsgListTagsReq: public Msg {
    void Run();
};

struct MsgListTagsResp: public Msg {
    void Run();

    std::vector<std::string> tagNames;
};

#endif
