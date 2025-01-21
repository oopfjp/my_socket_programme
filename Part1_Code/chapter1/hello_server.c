#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[]){
    if(3 != argc){
        printf("args error!\n");
    }

    // step1
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    printf("serv_sock = %d\n", serv_sock);

    if(serv_sock < 0){
        printf("sock error!\n");
    }

    // step2
    struct sockaddr_in serAddr, clienAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(serv_sock, (struct sockaddr* )&serAddr, sizeof(serAddr));
    if(ret == -1){
        printf("bind error!\n");
    }

    // step3
    if(listen(serv_sock, 5) == -1){
        printf("listen error!\n");
    }

    // step6
    socklen_t clienLen = sizeof(clienAddr);
    int clnt_sock;
    if((clnt_sock = accept(serv_sock, (struct sockaddr*) &clienAddr, &clienLen)) == 0){
        printf("accept error!\n");
    }


    // puts("connect success!");
    char* cliaddr = inet_ntoa(clienAddr.sin_addr);
    printf("client address: %s\n", cliaddr);
    unsigned cliePort = ntohs(clienAddr.sin_port);
    printf("client port = %u\n", cliePort);

    int bytes_num = 6;
    char message[] = "Hello?";
    if(write(clnt_sock, &bytes_num, sizeof(int)) == -1){
        printf("write() error\n");
    }

    if(write(clnt_sock, message, strlen(message)) == -1){
        printf("write() error\n");
    }
    // char buf[] = "Hello World\n";

    // for(int i = 0; i < 10; ++i){
    //     if(send(clnt_sock, buf, strlen(buf), 0) == -1){
    //         printf("send error!\n");
    //     }
    // }
    close(serv_sock);
    close(clnt_sock);
    puts("finished!");
}
    
