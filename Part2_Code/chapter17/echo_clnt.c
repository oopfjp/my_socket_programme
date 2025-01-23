#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <error.h>
#define BUF_SIZE 1024
#define EPOLL_SIZE 1024
#define ErrExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)
typedef struct sockaddr_in Addr_in;
typedef struct sockaddr Addr;

int main(int argc, char* argv[]){
    // .echo_clnt 127.0.0.1 1234

    if(3 != argc)
        ErrExit("args error");

    int sockfd = -1;
    Addr_in serv_adr;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
        ErrExit("socket");
    
    if(connect(sockfd, (Addr*)&serv_adr, sizeof(serv_adr)) == -1)
        ErrExit("connect");
    
    puts("connect success!");
    /*-------------------data transmission-------------------*/
    int epfd, event_cnt;
    struct epoll_event* ep_events;
    struct epoll_event event1, event2;
    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    event1.data.fd = 0;
    event2.data.fd = sockfd;
    event1.events = EPOLLIN | EPOLLET;
    event2.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event1);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event2);
    // fd_set fds, cpfds;
    int str_Len;
    // FD_ZERO(&fds);
    // FD_SET(0, &fds);
    // FD_SET(sockfd, &fds);
    char buf[BUF_SIZE];
    int i;
    while(1){
        // cpfds = fds;
        // select(sockfd+1, &cpfds, NULL, NULL, NULL);
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }

        for(i = 0; i < event_cnt; ++i){
            if(ep_events[i].data.fd == 0){
                memset(buf, 0, sizeof(buf));
                str_Len = read(0, buf, sizeof(buf) - 1);
                buf[str_Len] = 0;
                write(sockfd, buf, str_Len);
            }
            if(ep_events[i].data.fd == sockfd){
                memset(buf, 0, sizeof(buf));
                str_Len = read(sockfd, buf, sizeof(buf) - 1);
                buf[str_Len] = 0;
                fputs(buf, stdout);
            }
        }
        // if(FD_ISSET(0, &cpfds)){
        //     memset(buf, 0, sizeof(buf));
        //     str_Len = read(0, buf, sizeof(buf) - 1);
        //     buf[str_Len] = 0;
        //     write(sockfd, buf, str_Len);
        // }
        // if(FD_ISSET(sockfd, &cpfds)){
        //     memset(buf, 0, sizeof(buf));
        //     str_Len = read(sockfd, buf, sizeof(buf));
        //     buf[str_Len] = 0;
        //     fputs(buf, stdout);
        // }
    }
    close(sockfd);
    return 0;
}