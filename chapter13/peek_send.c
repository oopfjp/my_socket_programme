#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void error_handling(const char* message);
int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in send_adr;
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&send_adr, 0, sizeof(send_adr));
    send_adr.sin_family = AF_INET;
    send_adr.sin_port = htons(atoi(argv[2]));
    send_adr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr*)&send_adr, sizeof(send_adr)) == -1){
        error_handling("connect() error!");
    }

    write(sock, "123", strlen("123"));
    close(sock);
    return 0;
}


void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}