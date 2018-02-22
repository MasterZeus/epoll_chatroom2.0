//
// Created by jcduan on 18-1-23.
//

#ifndef TCPSERVER00_ERROR_HANDLER_H_H
#define TCPSERVER00_ERROR_HANDLER_H_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct sockaddr SA;

extern void error_exit(const char* errcode);//打印信息，异常终止进程
/*带有错误处理的socket操作函数*/
extern int Socket(int af, int stream, int protocol);
extern void Bind(int sockFd, SA * sockAddr, socklen_t addrLen);
extern void Listen(int fd, int logs);
extern int Accept(int fd, SA* sockAddr, socklen_t * addrLen);
extern void Connect(int fd, SA* sockAddr, socklen_t addrLen);

#ifdef __cplusplus
}
#endif

#endif //TCPSERVER00_ERROR_HANDLER_H_H
