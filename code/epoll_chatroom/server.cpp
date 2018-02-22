//
// Created by jcduan on 18-2-11.
//

#include "necessary.h"




//设置fd为非阻塞
int setNonblocking(int sockfd);
//注册事件和描述符
void addFd( int epollfd, int fd, enum epoll_mode mode );
//将服务器收到的clientfd的消息进行广播
int broadcastMessage(int clientfd);

struct threadArg {
    int epoll_events_count;
    struct epoll_event* events;
    int listener;
    int epfd;
};

void* thread_accetper(void* arg)
{
    struct threadArg* a = (struct threadArg*)arg;
    for(int i = 0; i < a->epoll_events_count; ++i)
    {
        int sockfd = a->events[i].data.fd;
        //新用户连接
        int clientfd;
        if(sockfd == a->listener)
        {
            struct sockaddr_in client_address;
            socklen_t client_addrLength = sizeof(struct sockaddr_in);

            for(;;)
            {
                clientfd = accept( a->listener, ( struct sockaddr* )&client_address, &client_addrLength );
                if(clientfd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                    break;

                printf("client connection from: %s : % d(IP : port), clientfd = %d \n",
                       inet_ntoa(client_address.sin_addr),
                       ntohs(client_address.sin_port),
                       clientfd);

                addFd(a->epfd, clientfd, ET);

                // 服务端用list保存用户连接
                clients_list.push_back(clientfd);
                printf("Add new clientfd = %d to epoll\n", clientfd);
                printf("Now there are %d clients int the chat room\n", (int)clients_list.size());

                // 服务端发送欢迎信息
                //   printf("welcome message\n");
                char message[BUF_SIZE];
                memset(message,0, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clientfd);
                writen(clientfd, message, BUF_SIZE);
            }



        }

    }

    pthread_exit((void*)"success!");
}

void* thread_boardcaster(void* arg)
{
    struct threadArg* a = (struct threadArg*)arg;
    for(int i = 0; i < a->epoll_events_count; ++i)
    {
        int sockfd = a->events[i].data.fd;
        //新用户连接
        if(sockfd != a->listener)
            if(broadcastMessage(sockfd)<0) error_exit("broadcastMessage!\n");

    }

    pthread_exit((void*)"success!");
}


int main(int argc, char *argv[])
{
    //服务器IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_IP,&serverAddr.sin_addr);
    //创建监听socket
    int listener = Socket(PF_INET, SOCK_STREAM, 0);

    printf("listener socket created \n");
    //绑定地址
    Bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    //监听
    Listen(listener, 10);
    printf("%s Start to listen\n", SERVER_IP);

    //listener 设为非阻塞
    setNonblocking(listener);

    //在内核中创建ep文件
    int epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0) error_exit("ep create!");
    printf("epoll created, epollfd = %d\n", epfd);

    //创建用户空间事件数组，用来收集就绪事件
    static struct epoll_event events[EPOLL_SIZE];
    //往ep文件里注册描述符和事件
    addFd(epfd, listener, ET);
    //thread id
    pthread_t accepter,boardcaster;
    char* thread_ret[2];

    //主循环
    while(1)
    {
        //epoll_events_count表示就绪事件的数目
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if(epoll_events_count < 0) {
            perror("epoll failure");
            break;
        }

        printf("epoll_events_count = %d\n", epoll_events_count);
        //处理这epoll_events_count个就绪事件
        threadArg arg ;
        arg.epoll_events_count = epoll_events_count;
        arg.epfd = epfd;
        arg.events = events;
        arg.listener =listener;
        pthread_create(&accepter, NULL, thread_accetper, (void*)&arg);
        pthread_create(&boardcaster, NULL, thread_boardcaster, (void*)&arg);

        pthread_join(accepter, (void**) &thread_ret[0]);
        pthread_join(boardcaster, (void** )&thread_ret[1]);
    }
    close(listener); //关闭socket
    close(epfd);    //关闭ep文件
    return 0;
}

//设置fd非阻塞
int setNonblocking(int sockfd)
{
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)| O_NONBLOCK);
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

//将服务器收到的clientfd的消息进行广播
int broadcastMessage(int clientfd)
{

    char buf[BUF_SIZE], message[BUF_SIZE];
    //清零
    bzero(buf, BUF_SIZE);
    bzero(message, BUF_SIZE);

    printf("read from client(clientID = %d)\n", clientfd);
    int len = recv(clientfd, buf, BUF_SIZE, 0);

    if(len == 0)  // len = 0 client关闭了连接
    {
        close(clientfd);
        clients_list.remove(clientfd);
        printf("ClientID = %d closed.\n now there are %d client in the char room\n", clientfd, (int)clients_list.size());

    }
    else  //进行广播
    {
        if(clients_list.size() == 1)
        {
            writen(clientfd, CAUTION, strlen(CAUTION));
            return len;
        }

        sprintf(message, SERVER_MESSAGE, clientfd, buf);

        std::list<int>::iterator it;
        for(it = clients_list.begin(); it != clients_list.end(); ++it)
        {
            if(*it != clientfd)
            {
                if( writen(*it, message, BUF_SIZE) < 0 ) { error_exit("writen!");}
            }
        }
    }
    return len;
}