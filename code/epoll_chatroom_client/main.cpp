#include "necessary.h"

//设置fd为非阻塞
int setNonblocking(int sockfd);
//注册事件和描述符
void addFd( int epollfd, int fd, enum epoll_mode mode );


int main(int argc, char *argv[])
{

    //用户连接的服务器 IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 创建socket
    int sock = Socket(PF_INET, SOCK_STREAM, 0);



    // 连接服务端
    Connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    //打印客户端ip
    struct sockaddr_in  client;
    socklen_t clientlen;
    getsockname(sock, (sockaddr*) &client, &clientlen);
    printf("this client ip is %s\n",inet_ntoa(client.sin_addr));

    // 创建管道，其中fd[0]用于父进程读，fd[1]用于子进程写
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0) { error_exit("pipe!") ;}

    // 创建epoll
    int epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0) { error_exit("epoll create") ;}
    static struct epoll_event events[2];
    //将sock和管道读端描述符都添加到内核事件表中
    addFd(epfd, sock, ET);
    addFd(epfd, pipe_fd[0], ET);
    // 表示客户端是否正常工作
    bool isClientwork = true;

    // 聊天信息缓冲区
    char message[BUF_SIZE];

    // Fork
    int pid = fork();
    if(pid < 0) { error_exit("fork!") ;}
    else if(pid == 0)      // 子进程
    {
        //子进程负责写入管道，因此先关闭读端
        close(pipe_fd[0]);
        printf("Please input 'exit' to exit the chat room\n");

        while(isClientwork){
            memset(message, 0, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            // 客户输出exit,退出
            if(strncasecmp(message, EXIT, strlen(EXIT)) == 0){
                isClientwork = false;
            }
                // 子进程将信息写入管道
            else {
                if( write(pipe_fd[1], message, strlen(message) - 1 ) < 0 )
                { error_exit("write!");}
            }
        }
    }
    else  //pid > 0 父进程
    {
        //父进程负责读管道数据，因此先关闭写端
        close(pipe_fd[1]);

        // 主循环(epoll_wait)
        while(isClientwork) {
            int epoll_events_count = epoll_wait( epfd, events, 2, -1 );
            //处理就绪事件
            for(int i = 0; i < epoll_events_count ; ++i)
            {
                bzero(&message, BUF_SIZE);

                //服务端发来消息
                if(events[i].data.fd == sock)
                {
                    //接受服务端消息
                    int ret = recv(sock, message, BUF_SIZE, 0);

                    // ret= 0 服务端关闭
                    if(ret == 0) {
                        printf("Server closed connection: %d\n", sock);
                        close(sock);
                        isClientwork = 0;
                    }
                    else printf("%s\n", message);

                }
                    //子进程写入事件发生，父进程处理并发送服务端
                else {
                    //父进程从管道中读取数据
                    int ret = readn(events[i].data.fd, message, BUF_SIZE);

                    // ret = 0
                    if(ret == 0) isClientwork = 0;
                    else{   // 将信息发送给服务端
                        writen(sock, message, BUF_SIZE);
                    }
                }
            }//for
        }//while
    }

    if(pid){
        //关闭父进程管道和sock
        close(pipe_fd[0]);
        close(sock);
    }else{
        //关闭子进程管道
        close(pipe_fd[1]);
    }
    return 0;
}

void addFd( int epollfd, int fd, enum epoll_mode mode )
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;     //输入触发epoll-event
    if( mode == ET )
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setNonblocking(fd);
    //   printf("fd added to epoll!\n\n");
}

//设置fd非阻塞
int setNonblocking(int sockfd)
{
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)| O_NONBLOCK);
    return 0;
}

