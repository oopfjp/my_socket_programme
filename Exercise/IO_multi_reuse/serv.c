#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void errorHandling(const char* message);
int main(int argc, char* argv[]){

    if(argc!=3){
        errorHandling("args error!");
    }

    int serv_sock, cln_sock;
    struct sockaddr_in serv_adr, cln_adr;
    char buf[BUF_SIZE];
    int str_len;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        errorHandling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        errorHandling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        errorHandling("listen() error");
    }
    socklen_t cln_adr_sz = sizeof(cln_adr);
    cln_sock = accept(serv_sock, (struct sockaddr*)&cln_adr, &cln_adr_sz);
    if(cln_sock == -1){
        errorHandling("accept() error");
    }

    puts("connect success");
    fd_set fds, cpfds;
    FD_ZERO(&fds);
    FD_SET(cln_sock, &fds);
    FD_SET(0, &fds);
    
    
    while(1){
        cpfds = fds;
        select(cln_sock+1, &cpfds, NULL, NULL, NULL);
        if(FD_ISSET(cln_sock, &cpfds)){
            memset(buf, 0, sizeof(buf));
            if((str_len = read(cln_sock, buf, BUF_SIZE)) != -1){
                buf[str_len] = 0;
                fputs(buf, stdout);
            }
        }else if(FD_ISSET(0, &fds)){
            memset(buf, 0, sizeof(buf));
            if((str_len = read(0, buf, sizeof(buf))) != -1){
                buf[str_len] = 0;
                write(cln_sock, buf, str_len);
            }
        }
    }

    close(serv_sock);
    close(cln_sock);     
    
    return 0;
}

void errorHandling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
}