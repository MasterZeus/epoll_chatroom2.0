//
// Created by jcduan on 18-1-23.
//

#include "error_handler.h"

void error_exit(const char* errcode)
{
    printf("error: %s\n", errcode);
    exit(1);
}

int Socket(int af, int stream, int protocol)
{
    int fd;
    if((fd = socket(af,stream,protocol)) < 0)
        error_exit("socket error!");
    return  fd;
}
void Bind(int sockFd, SA * sockAddr, socklen_t addrLen)
{
    if(bind(sockFd, sockAddr, addrLen) < 0)
        error_exit("bind error!");
}
void Listen(int fd, int logs)
{
    if(listen(fd,logs) < 0)
        error_exit("listen error!");
}
int Accept(int fd, SA* sockAddr, socklen_t * addrLen)
{
    int tfd;
    if( (tfd = accept(fd, sockAddr,addrLen)) < 0)
        error_exit("accept error!");
    return tfd;
}


void Connect(int fd, SA *sockAddr, socklen_t addrLen) {
    if(connect(fd, sockAddr, addrLen)< 0)
        error_exit("connect error!");
}

