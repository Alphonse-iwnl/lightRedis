#ifndef _REACTOR_H
#define _REACTOR_H
#include<sys/epoll.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/unistd.h>
#include<string.h>
#include<stdio.h>
#include"../command/controller.h"
#include"../command/converter.h"

#define MAXLINE 2048
#define FDSIZE 1000
#define EPOLLEVENTS 100

class reactor{
    int epollfd,listenfd,clifd,ret;
    std::map<int,std::string> clibuf;
    converter cv;
    controller ctl; 

    FILE *RW;
    char *buf;

    struct epoll_event events[EPOLLEVENTS];
    
    //change:register/update/del
    //state:event type
    void event_controll(const int &epollfd,const int &fd,const int &state,const int &change);
    //para for epoll_wait
    void event_demultiplexer(const int &epollfd,struct epoll_event *events,const int &ret,const int &listenfd);
    //handle
    void listen_handle(const int &epollfd,const int fd);
    void read_handle(const int &epollfd,const int fd);
    void write_handle(const int &epollfd,const int fd);

    public:
    reactor(int &_epollfd,int _listenfd);
    void reactor_listen(const int &timeout);
    void aof();
};
#endif
