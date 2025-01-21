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
    int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    printf("sock_fd = %d\n", sock_fd);
    if(sock_fd < 0){
        printf("sock error!\n");
    }

    // step2
    struct sockaddr_in serAddr, clienAddr;
    serAddr.sin_family = PF_INET;//ipv4
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(sock_fd, (struct sockaddr* )&serAddr, sizeof(serAddr));
    if(ret == -1){
        printf("bind error!\n");
    }

    // step3
    if(listen(sock_fd, 5) == -1){
        printf("listen error!\n");
    }

    // step4
    socklen_t clienLen = sizeof(clienAddr);
    int clien_fd;
    if((clien_fd = accept(sock_fd, (struct sockaddr*) &clienAddr, &clienLen)) == -1){
        printf("accept error!\n");
    }

    puts("connect success!");
    
    char buf[] = "Hello World\n";

    for(int i = 0; i < 10; ++i){
        if(send(clien_fd, buf, strlen(buf), 0) == -1){
            printf("send error!\n");
            break;
        }
    }
    close(sock_fd);
    close(clien_fd);
    puts("finished!");
}
    
