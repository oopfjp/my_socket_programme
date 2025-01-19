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
    int str_len;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr, from_adr;

    sock = socket(AF_INET, SOCK_DGRAM, 0); //create UDP socket
    if(sock == -1){
        error_handling("sock() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    while (1)
    {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
            break;
        }
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        // sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        adr_sz = sizeof(from_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
        message[str_len] = 0;
        printf("Message from server: %s", message);

        char* str_ptr;
        str_ptr = inet_ntoa(from_adr.sin_addr);
        unsigned short from_port = ntohs(from_adr.sin_port);
        printf("from_adr = %s\n", str_ptr);
        printf("from_port = %u\n", from_port);

    }
    close(sock);
    return 0;
}

void error_handling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
    exit(1);
}