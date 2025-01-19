#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>

#define BUF_SIZE 1024
#define BUF_SIZE 1024

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
    
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // step3
    if(connect(serv_sock, (struct sockaddr*)&serAddr, sizeof(serAddr)) == -1){
        printf("connect error!\n");
    }
    int bytes_num;
    if(read(serv_sock, &bytes_num, sizeof(int)) == -1){
        printf("read() error\n");
    }
    printf("bytes_num = %d\n", bytes_num);

    char message[BUF_SIZE];
    int read_sz = 0; 
    while(read_sz < bytes_num){
        int readSize = read(serv_sock, &message[read_sz], BUF_SIZE);
        printf("readSize = %d\n", readSize);
        read_sz += readSize;
    }
    message[bytes_num] = 0;
    fputs(message, stdout);
    // puts("connect success!");
    
    // char buf[1024] = {0};


    // // sleep(1000);
    // for(int i = 0; i < 3000; ++i){
    //     printf("Wait time%d \n", i);
    // }
    // if(recv(serv_sock, buf, sizeof(buf) - 1, 0) == -1){
    //     printf("recv error!\n");
    // }

    // printf("%s\n", buf);
    
    close(serv_sock);
}


