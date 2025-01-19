#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#define BUF_SIZE 30
void error_handling(const char* message);
int main(int argc, char* argv[]){
    if(3!=argc){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int sock; 
    struct sockaddr_in recv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(atoi(argv[2]));
    recv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1){
        error_handling("connect() error!");
    }

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);
    return 0;

}

void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}