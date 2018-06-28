#include"reactor.h"

reactor::reactor(int &_epollfd,int _listenfd){
    this->epollfd=epoll_create(FDSIZE);
    _epollfd=epollfd;
    listenfd=_listenfd;

    buf=new char[MAXLINE];
    memset(buf,0,MAXLINE);
    event_controll(_epollfd,_listenfd,EPOLLIN,EPOLL_CTL_ADD);
}

void reactor::aof(){//read command
    RW=fopen("aof","a+");
    while(fgets(buf,MAXLINE,RW)){
        ctl.handleCommand(cv.translateCommand2data(std::string(buf)));
    }      
    memset(buf,0,MAXLINE);
    fclose(RW);
}

void reactor::reactor_listen(const int &timeout){
    for(;;){
        reactor::ret=epoll_wait(epollfd,events,EPOLLEVENTS,timeout);
        reactor::event_demultiplexer(epollfd,events,ret,listenfd);
    }
}

void reactor::event_controll(const int &epollfd,const int &fd,const int &state,const int &change){
    struct epoll_event evt;
    evt.events=state;
    evt.data.fd=fd;
    epoll_ctl(epollfd,change,fd,&evt);
}

void reactor::event_demultiplexer(const int &epollfd,struct epoll_event *events,const int &ret,const int &listenfd){
    int i,fd;
    for(i=0;i<ret;i++){
        fd=events[i].data.fd;
        if((fd==listenfd)&&events[i].events&EPOLLIN)
            reactor::listen_handle(epollfd,fd);
        else if(events[i].events&EPOLLIN)
            reactor::read_handle(epollfd,fd);
        else if(events[i].events&EPOLLOUT)
            reactor::write_handle(epollfd,fd);
    }
}

void reactor::listen_handle(const int &epollfd,const int fd){
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    clifd=accept(fd,(struct sockaddr *)&cliaddr,&clilen);
    if(clifd!=-1)
        this->event_controll(epollfd,clifd,EPOLLIN,EPOLL_CTL_ADD);
}

void reactor::read_handle(const int &epollfd,const int fd){
    int nread=0;
    nread=read(fd,buf,MAXLINE);
    if(nread==-1||nread==0){//eof
        close(fd);
        reactor::event_controll(epollfd,fd,EPOLLIN,EPOLL_CTL_DEL);
    }else {
        std::string response=ctl.handleCommand(cv.translateCommand2data(std::string(buf)));
        std::string fresponse=std::string(buf);
        fresponse+="\n";
        char *cm=new char[fresponse.length()+1];
        strcpy(cm,fresponse.c_str());
        RW=fopen("aof","a+");
        fputs(cm,RW);//write to aof file 
        fclose(RW);
        reactor::clibuf.insert(std::pair<int,std::string>(fd,response));
        reactor::event_controll(epollfd,fd,EPOLLOUT,EPOLL_CTL_MOD);
    }
    memset(buf,0,MAXLINE);
}

void reactor::write_handle(const int &epollfd,const int fd){
    int nwrite;
    std::string response;
    auto it=clibuf.find(fd);
    if(it!=clibuf.end()){
        nwrite=write(fd,((std::string)(it->second)).c_str(),((std::string)(it->second)).length());
        if(nwrite==-1){
            close(fd);
            reactor::event_controll(epollfd,fd,EPOLLOUT,EPOLL_CTL_DEL);
        } else{
            reactor::event_controll(epollfd,fd,EPOLLIN,EPOLL_CTL_MOD);
        }
    }
}
