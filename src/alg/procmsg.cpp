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
#include "alg.h"
#include "store/store.h"
#include "msg/msg.h"

static bool Init()
{
    return Alg::Instance()->Init();
}

static bool Exit()
{
    return Alg::Instance()->Release();
}

MODULE(ALG, Init, Exit);

class DetectTask: public Task {
private:
    std::string m_imgFile;
public:
    DetectTask(const char *imgFile): m_imgFile(imgFile) {}
    void Run() {
        FaceSource src;
        DetectResults results;

        src.imgFile = m_imgFile;
        Alg::Instance()->Detect(src, results);
        LOG_INFO("%s detected %zd faces", m_imgFile.c_str(), results.size());

        for (auto &r : results) {
            Rect &rc = r.rc;
            LOG_INFO("Detect %d %d %d %d", (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);
        }
    }
};

class ExtractTask: public Task {
private:
    std::string m_tagName;
    std::string m_imgFile;
public:
    ExtractTask(const char *tagName, const char *imgFile): m_tagName(tagName), m_imgFile(imgFile) {}
    void Run() {
        FaceSource src;
        ExtractResults results;

        src.imgFile = m_imgFile;
        Alg::Instance()->Extract(src, results);

        //LOG_INFO("%s extract %zd faces", m_imgFile.c_str(), results.size());
        Store::Instance()->AddFaces(m_tagName, results);
    }
};

void MsgDetectReq::Run()
{
}

void MsgCheckinReq::Run()
{
}
