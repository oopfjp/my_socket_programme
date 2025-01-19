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
    char message[BUF_SIZE];
    int str_len, i;
    socklen_t adr_sz;
    struct sockaddr_in my_addr, your_addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0); //create an UDP socket
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_port = htons(atoi(argv[2]));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
        error_handling("bind() error");
    }

    for(i = 0; i < 3; ++i){
        sleep(5);
        adr_sz = sizeof(your_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&your_addr, &adr_sz);
        printf("Message %d: %s \n", i+1, message);
    }
    close(sock);
    return 0;
}

void error_handling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
    exit(1);
}