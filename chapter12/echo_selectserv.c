#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#define BUF_SIZE 1024
void error_handling(const char* message);
int main(int argc, char* argv[]){
    if(3!=argc){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int serv_sock, cln_sock;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == serv_sock){
        error_handling("socket() error");
    }
    int opt = 8;
    socklen_t option;
    option = sizeof(opt);
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, option);
    struct sockaddr_in serv_addr, cln_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret;
    socklen_t adr_sz;
    ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(-1 == ret){
        error_handling("bind() error");
    }
    ret = listen(serv_sock, 8);
    if(-1 == ret){
        error_handling("listen() error");
    }

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    //data transmission
    
    
    
    while(1){

        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
            break;
        }
        if(fd_num = 0){
            continue; //超时了
        }
        for(i = 0; i < fd_max + 1; ++i){
            if(FD_ISSET(i, &cpy_reads)){
                if(i == serv_sock){ //connect request!
                    adr_sz = sizeof(cln_addr);
                    cln_sock = accept(serv_sock, (struct sockaddr*)&cln_addr, &adr_sz);
                    FD_SET(cln_sock, &reads);
                    if(fd_max < cln_sock){
                        fd_max = cln_sock;
                    }
                    printf("connected clinet: %d \n", cln_sock);
                }else{ // read message!
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0){
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    }else{
                        write(i, buf, str_len); // echo!
                    }

                }
            }
        }
    }
        
    close(serv_sock);
    return 0;
    // close(cln_sock);
}

void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}

    