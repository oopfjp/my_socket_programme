#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#define BUF_SIZE 1024
void error_handling(const char* message);
int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;
    struct sockaddr_in serAddr, clienAddr;
    socklen_t clnt_addr_sz;
    
    if(argc!=2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    // step1
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        printf("sock error!\n");
    }

    // step2
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[1]));
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(serv_sock, (struct sockaddr* )&serAddr, sizeof(serAddr));
    if(ret == -1){
       error_handling("bind() error");
    }

    // step3
    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    // step6
    clnt_addr_sz = sizeof(clienAddr);
    for(int i = 0; i < 5; ++i){//
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clienAddr, &clnt_addr_sz);
        if(clnt_sock == -1){
            error_handling("accept() error");
        }else{
            printf("Connect client %d \n", i+1);
        }
        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0){
            write(clnt_sock, message, str_len);
        }
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;


    // puts("connect success!");
    // char* cliaddr = inet_ntoa(clienAddr.sin_addr);
    // printf("client address: %s\n", cliaddr);
    // unsigned cliePort = ntohs(clienAddr.sin_port);
    // printf("client port = %u\n", cliePort);

    // close(serv_sock);
    // close(clnt_sock);
    // puts("finished!");
}
    
void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}