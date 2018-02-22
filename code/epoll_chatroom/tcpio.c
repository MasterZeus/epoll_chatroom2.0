//
// Created by jcduan on 18-1-23.
//

#include "tcpio.h"

/*从缓冲区中读取数据或者将数据写入缓冲区中*/
int readn(int fd, void* ptr, int n)
{
    /*从缓冲区中读取n个字节的数据
     * 如果n为0或者负数，则当作读0个字节数据来处理。
     * 如n为正整数，则读取n个字节。
     * 如缓冲区数据不够，则只读完所有数据。
     * 最后返回已读取字节数。
     * */

    char* p = (char*)ptr;//指向空余空间
    int nleft = n;//剩余要读取的字符数
    int nread = 0;//已读取字符数

    while(nleft > 0)
    {
        nread = read(fd, p, nleft);
        if(nread == 0)
            break;//缓冲区中没数据

        if(nread < 0)
        {
            if(errno == EINTR)
                nread = 0;//如果是信号中断了系统调用，视为执行了一次0字节读入。
            else
                return -1;
        }

        nleft -= nread;
        p += nread;
    }
    return n-nleft;
}

int writen(int fd,const void* ptr, int n)
{
    /*向缓冲区中写入n字节数据
     * 如果n为0或者负数，则写入0字节
     * 如果n为正整数则写入n字节
     * 最后返回写入字节数
     * */

    const char* p = (char*)ptr;
    int nleft = n;
    int nwritten = 0;

    while(nleft > 0)
    {
        nwritten = write(fd, p, nleft);
        if(nwritten <=0)
        {
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        p += nwritten;


    }

    return n>0 ? n: 0;
}


