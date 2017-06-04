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

class Msg: public Task {
public:
    int sess;
};

struct MsgDetectReq: public Msg {
    void Run();

    std::vector<std::string> imgFiles;
};

struct MsgDetectResp: public Msg {
    void Run();

    struct result {
        std::string imgFile;
        int x,y,w,h;
    };
    std::vector<result> results;
};

struct MsgSearchTagReq: public Msg {
    void Run();

    FaceID faceID;
    std::string tagName;
};

struct MsgSearchTagResp: public Msg {
    void Run();

    FaceID srcFaceID;
    struct result {
        FaceID matchFaceID;
        float score;
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

    bool success;
};

class MsgStoreAddFaces: public Msg {
public:
    void Run();
    std::string tagName;
    const ExtractResults results;
};

class MsgStoreListTags: public Msg {
public:
    void Run();
};

#endif
