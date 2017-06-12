/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: server.cpp
 *     Created: 2017-06-12 18:23
 * Description:
 * ===================================================
 */
#include <assert.h>
#include <signal.h>
#include <thread>
#include "common/common.h"
#include "socket/socket_server.h"

static socket_server *gss = NULL;

struct Head {
    uint32_t size;
    uint32_t type;
};

static void *
_poll(void * ud) {
    struct socket_server *ss = (struct socket_server*)ud;
    struct socket_message result;
    for (;;) {
        int type = socket_server_poll(ss, &result, NULL);
        switch (type) {
        case SOCKET_EXIT:
            return NULL;
        case SOCKET_DATA:
            LOG_INFO("message(%lu) [id=%d] size=%d",result.opaque,result.id, result.ud);
            free(result.data);
            break;
        case SOCKET_CLOSE:
            LOG_INFO("close(%lu) [id=%d]",result.opaque,result.id);
            break;
        case SOCKET_OPEN:
            LOG_INFO("open(%lu) [id=%d] %s",result.opaque,result.id,result.data);
            break;
        case SOCKET_ERROR:
            LOG_INFO("error(%lu) [id=%d]",result.opaque,result.id);
            break;
        case SOCKET_ACCEPT:
            LOG_INFO("accept(%lu) [id=%d %s] from [%d]",result.opaque, result.ud, result.data, result.id);
            break;
        }
    }
}

void DisableSignalPipe()
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);
}

int ServerStart()
{
    struct socket_server * ss = socket_server_create();
    assert(ss);

    std::thread t1(_poll, ss);
    t1.detach();
    DisableSignalPipe();

    gss = ss;
    int sid = socket_server_listen(ss, 200, "127.0.0.1", 8888, 32);
    if (sid < 0) {
        LOG_ERROR("listen failed");
        return -1;
    }

    socket_server_start(ss, 0, sid);

    return 0;
}

int ServerStop()
{
    socket_server_exit(gss);
    socket_server_release(gss);
    return 0;
}
