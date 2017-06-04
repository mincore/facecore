/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: test/test.cpp
 *     Created: 2017-06-01 19:15
 * Description:
 * ===================================================
 */
#include <strings.h>
#include "module/module.h"
#include "msg/msg.h"
#include "test.h"
#include "store/store.h"

extern Module *MODULE_ALG;
extern Module *MODULE_STORE;
extern Module *MODULE_GATE;

typedef std::vector<std::string> ARGS;

static bool test_help(ARGS &args);
static bool test_exit(ARGS &args);
static bool test_detect(ARGS &args);
static bool test_add(ARGS &args);
static bool test_show(ARGS &args);

struct test_table {
    const char *cmd;
    bool (*proc)(ARGS &args);
    const char *desc;
} g_test_table[] = {
    {"help", test_help, "print help"},
    {"exit", test_exit, "exit"},
    {"detect", test_detect, "detect N: detect N images"},
    {"add", test_add, "add group N: extrace N images into a group"},
    {"show", test_show, "show group: show all faces in group"},
};

bool run_test(const char *line)
{
    ARGS args;
    strsplit(line, ' ', args);
    if (args.size() == 0)
        return true;

    for (auto &i : g_test_table) {
        if (strcasecmp(i.cmd, args[0].c_str()) == 0) {
            return i.proc(args);
        }
    }
    return true;
}

static bool test_exit(ARGS &args)
{
    return false;
}

static bool test_help(ARGS &args)
{
    for (auto &i : g_test_table) {
        printf("%8s : %s\n", i.cmd, i.desc);
    }
    return true;
}

static bool test_detect(ARGS &args)
{
    char tmpPath[32];
    int n = 1;
    if (args.size() > 1)
        n = atoi(args[1].c_str());

    MsgDetectReq *req = new MsgDetectReq;
    for (int i=0; i<n; i++) {
        sprintf(tmpPath, "/detect/%d.jpg", n);
        req->imgFiles.push_back(tmpPath);
    }
    MODULE_ALG->Push(req);

    return true;
}

static bool test_add(ARGS &args)
{
    if (args.size() != 3)
        return true;

    int n = atoi(args[2].c_str());
    char tmpPath[32];

    MsgAddImgReq *req = new MsgAddImgReq;
    req->tagName = args[1];
    for (int i=0; i<n; i++) {
        sprintf(tmpPath, "/extract/%s_%i.jpg", args[1].c_str(), i);
        req->imgFiles.push_back(tmpPath);
    }
    MODULE_ALG->Push(req);

    return true;
}

static bool test_show(ARGS &args)
{
    if (args.size() == 1) {
        std::vector<std::string> tagNames;
        Store::Instance()->ListTags(tagNames);
        LOG_INFO("has %zd tags", tagNames.size());
        for (auto &i : tagNames) {
            LOG_INFO("tag: %s", i.c_str());
        }
        return true;
    }

    auto &tagName = args[1];
    std::list<FaceID> faceIDs;
    Store::Instance()->ListFaces(tagName, faceIDs);
    LOG_INFO("%s has %zd faceIDs. [%lld-%lld]", tagName.c_str(), faceIDs.size(),
            (long long)faceIDs.front(), (long long)faceIDs.back());

    return true;
}
