#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#define BUF_SIZE 1024
#define ErrExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)
typedef struct sockaddr_in Addr_in;
typedef struct sockaddr Addr;

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
    fd_set fds, cpfds;
    int str_Len;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    FD_SET(cln_fd, &fds);
    char buf[BUF_SIZE];
    while(1){
        cpfds = fds;
        select(cln_fd+1, &cpfds, NULL, NULL, NULL);
        if(FD_ISSET(cln_fd, &cpfds)){
            memset(buf, 0, sizeof(buf));
            str_Len = read(cln_fd, buf, sizeof(buf));
            if(str_Len == 0){
                break;
            }
            buf[str_Len] = 0;
            write(cln_fd, buf, str_Len);
        }
    }
    close(sockfd);
    close(cln_fd);
    return 0;
}