//
// Created by jcduan on 18-1-23.
//

#ifndef TCPSERVER00_TCPIO_H
#define TCPSERVER00_TCPIO_H

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

/*从缓冲区中读取数据或者将数据写入缓冲区中*/

extern int readn(int fd, void *ptr, int n);

extern int writen(int fd, const void *ptr, int n);

#ifdef __cplusplus
}
#endif

#endif //TCPSERVER00_TCPIO_H
