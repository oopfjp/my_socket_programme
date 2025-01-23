#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <error.h>
#define BUF_SIZE 4
#define EPOLL_SIZE 1024
#define ErrExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)
typedef struct sockaddr_in Addr_in;
typedef struct sockaddr Addr;
void setnonblockingmode(int fd);
int main(int argc, char* argv[]){
    // .echo_clnt 127.0.0.1 1234

    if(3 != argc)
        ErrExit("args");

    int sockfd = -1, cln_fd;
    Addr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz = sizeof(clnt_adr);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
        ErrExit("socket");

    //set sockfd
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));
    

    if(bind(sockfd, (Addr*)&serv_adr, sizeof(serv_adr)) == -1)
        ErrExit("bind() error");
    
    if(listen(sockfd, 5) == -1)
        ErrExit("listen() error");
    
    cln_fd = accept(sockfd, (Addr*)&clnt_adr, &clnt_adr_sz);
    if(-1 == cln_fd)
        ErrExit("accept() error");
    
    puts("connect success!");

    /*-------------------data transmission-------------------*/
    // fd_set fds, cpfds;
    int epfd, event_cnt;
    struct epoll_event* ep_events;
    struct epoll_event event;
    epfd = epoll_create(EPOLL_SIZE);
    // printf("epfd = %d\n", epfd);
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE); //这里是乘法
    event.data.fd = cln_fd;
    setnonblockingmode(cln_fd);
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, cln_fd, &event);
    int str_Len;
    // FD_ZERO(&fds);
    // FD_SET(0, &fds);
    // FD_SET(cln_fd, &fds);
    char buf[BUF_SIZE];
    int i;
    while(1){
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        // printf("evenct_cnt = %d\n", event_cnt);
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }
        puts("return epoll_wait");
        for(i = 0; i < event_cnt; i++){
            if(ep_events[i].data.fd == cln_fd){
                while(1){
                    memset(buf, 0, sizeof(buf));
                    str_Len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_Len == 0){
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d \n", ep_events[i].data.fd);
                        break;
                    }else if(str_Len < 0){
                        // if(errno == EAGAIN)
                            break;
                    }else{
                        write(ep_events[i].data.fd, buf, str_Len);
                    }
                    
                }
                
            }
                
        }
        // cpfds = fds;
        // select(cln_fd+1, &cpfds, NULL, NULL, NULL);
        // if(FD_ISSET(cln_fd, &cpfds)){
        //     memset(buf, 0, sizeof(buf));
        //     str_Len = read(cln_fd, buf, sizeof(buf));
        //     if(str_Len == 0){
        //         break;
        //     }
        //     buf[str_Len] = 0;
        //     write(cln_fd, buf, str_Len);
        // }
    }
    
    close(sockfd);
    close(epfd);
    // close(cln_fd);
    return 0;
}

void setnonblockingmode(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}