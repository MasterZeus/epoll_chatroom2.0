//
// Created by jcduan on 18-2-11.
//
/*包含了必要的c/c++头文件
 * 以及一些常量定义
 */
#ifndef EPOLL_CHATROOM_NECESSARY_H
#define EPOLL_CHATROOM_NECESSARY_H

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "error_handler.h"
#include "tcpio.h"

#ifdef __cplusplus
}
#endif

#ifdef  __cplusplus

#include <iostream>
#include <list>



// client_list
std::list<int> clients_list;


// 服务器ip
static const char*  const SERVER_IP = "127.0.0.1";

// server port
static const int SERVER_PORT  = 8888;

//epoll 支持的最大并发量
static const int EPOLL_SIZE =5000;

//message buffer size
static const int BUF_SIZE =0xFFFF;

static const char* const SERVER_WELCOME ="Welcome you join  to the chat room! Your chat ID is: Client #%d";

static const char* const SERVER_MESSAGE ="ClientID %d say >> %s";

// exit
static const char* const EXIT ="EXIT";

static const char* const CAUTION ="There is only one int the char room!";

enum epoll_mode{ET, LT};



#endif //__cplusplus

#endif //EPOLL_CHATROOM_NECESSARY_H
