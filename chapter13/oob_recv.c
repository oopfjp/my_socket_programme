#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <fcntl.h>
#define BUF_SIZE 30
void error_handling(const char* message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char* argv[]){
    struct sockaddr_in recv_adr, serv_adr;
    int str_len, state;
    socklen_t serv_adr_sz;
    struct sigaction act;
    char buf[BUF_SIZE];

    if(3!=argc){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));


    
    if(bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1){
        error_handling("bind() error");
    }
    listen(acpt_sock, 5);
    
    serv_adr_sz = sizeof(serv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

    fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while((str_len = recv(recv_sock, buf, sizeof(buf), 0))!=0){
        if(str_len == -1){
            continue;
        }
        buf[str_len] = 0;
        puts(buf);
    }
    
    close(recv_sock);
    close(acpt_sock);
    return 0;
    // close(cln_sock);
}
void urg_handler(int signo){
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s \n", buf);
}
void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}

    