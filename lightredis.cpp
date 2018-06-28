#include"reactor/reactor.h"

int main(){
      int epollfd,listenfd;
      struct sockaddr_in servaddr;

      listenfd=socket(AF_INET,SOCK_STREAM,0);

      if(listenfd!=-1){
            bzero(&servaddr,sizeof(servaddr));
            servaddr.sin_family=AF_INET;
            servaddr.sin_port=htons(9877);
            servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

            bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
      }

      listen(listenfd,1024);//backlog

      reactor react(epollfd,listenfd);
      //react.aof();
      react.reactor_listen(-1);

      close(epollfd);
}
