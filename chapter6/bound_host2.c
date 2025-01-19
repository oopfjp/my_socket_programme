#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BUF_SIZE 30
void error_handling(const char* message);
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    int sock;//类似于建立信箱
    char msg1[] = "Hi!";
    char msg2[] = "I am another UDP host!";
    char msg3[] = "Nice to meet you";
    socklen_t adr_sz;
    struct sockaddr_in your_adr;

    sock = socket(AF_INET, SOCK_DGRAM, 0); //create UDP socket
    if(sock == -1){
        error_handling("sock() error");
    }

    memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_port = htons(atoi(argv[2]));
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);

    sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
    sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
    sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
    close(sock);
    return 0;
}

void error_handling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
    exit(1);
}