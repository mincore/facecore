/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: test/test.cpp
 *     Created: 2017-06-01 19:15
 * Description:
 * ===================================================
 */
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "module/module.h"
#include "msg/msg.h"
#include "test.h"
#include "store/store.h"

extern Module *MODULE_ALG;
extern Module *MODULE_STORE;
extern Module *MODULE_GATE;

typedef std::vector<std::string> ARGS;

static bool run_one_test(ARGS &args);
static void test_help(ARGS &args);
static void test_exit(ARGS &args);
static void test_detect(ARGS &args);
static void test_add(ARGS &args);
static void test_show(ARGS &args);
static void test_search(ARGS &args);

struct test_table {
    const char *cmd;
    void (*proc)(ARGS &args);
    const char *desc;
} g_test_table[] = {
    {"help"   , test_help   , "print help"}                                        ,
    {"exit"   , test_exit   , "exit"}                                              ,
    {"detect" , test_detect , "detect N: detect N images"}                         ,
    {"add"    , test_add    , "add group N: extrace N images into a group"}        ,
    {"search" , test_search , "search group picfile: search a picture in a group"} ,
    {"show"   , test_show   , "show group: show all faces in group"}               ,
};
static bool g_quit = false;

void run_tests()
{
    ARGS args;
    char *line;

    while (!g_quit) {
        line = readline("DEBUG> ");

        strsplit(line, ' ', args);
        if (run_one_test(args))
            add_history(line);

        free(line);
    }
}

static bool run_one_test(ARGS &args)
{
    if (args.size() == 0)
        return false;

    for (auto &i : g_test_table) {
        if (strcasecmp(i.cmd, args[0].c_str()) == 0) {
            i.proc(args);
            return true;
        }
    }
    return false;
}

static void test_exit(ARGS &args)
{
    g_quit = true;
}

static void test_help(ARGS &args)
{
    for (auto &i : g_test_table) {
        printf("%8s : %s\n", i.cmd, i.desc);
    }
}

static void test_detect(ARGS &args)
{
    int n = 1;
    if (args.size() > 1)
        n = atoi(args[1].c_str());

    for (int i=0; i<n; i++) {
        MsgDetectReq *req = new MsgDetectReq;
        MODULE_ALG->Push(req);
    }

}

static void test_add(ARGS &args)
{
    if (args.size() != 3)
        return;

    //int n = atoi(args[2].c_str());

    MsgCheckinReq *req = new MsgCheckinReq;
    MODULE_ALG->Push(req);
}

static void test_show(ARGS &args)
{
    if (args.size() == 1) {
        std::vector<std::string> tagNames;
        Store::Instance()->ListTags(tagNames);
        LOG_INFO("has %zd tags", tagNames.size());
        for (auto &i : tagNames) {
            LOG_INFO("tag: %s", i.c_str());
        }
        return;
    }

    auto &tagName = args[1];
    size_t count = Store::Instance()->GetTagFaceCount(tagName);
    LOG_INFO("%s has %zd faceIDs", tagName.c_str(), count);
}

static void test_search(ARGS &args)
{
    if (args.size() != 3)
        return;

    MsgSearchReq *req = new MsgSearchReq;
    MODULE_ALG->Push(req);
}
