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
    int serv_sock;//类似于建立信箱
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr, cln_adr;

    serv_sock = socket(AF_INET, SOCK_DGRAM, 0); //create an UDP socket
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }

    while (1)
    {
        clnt_adr_sz = sizeof(cln_adr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&cln_adr, &clnt_adr_sz);
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&cln_adr, clnt_adr_sz);
        char* str_ptr;
        str_ptr = inet_ntoa(cln_adr.sin_addr);
        unsigned short from_port = ntohs(cln_adr.sin_port);
        printf("from_adr = %s\n", str_ptr);
        printf("from_port = %u\n", from_port);
    }
    close(serv_sock);
    return 0;
}

void error_handling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
    exit(1);
}