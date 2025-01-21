#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#define BUF_SIZE 30
void error_handling(const char* message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char* argv[]){
    
    int acpt_sock, recv_sock;
    struct sockaddr_in acpt_adr, recv_adr;
    int str_len, state; 
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_port = htons(atoi(argv[2]));
    acpt_adr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(acpt_sock, (struct sockaddr*)&acpt_adr, sizeof(acpt_adr)) == -1){
        error_handling("bind() error");
    }
    listen(acpt_sock, 5);
    
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);

    while (1)
    {
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        if(str_len > 0){
            break;
        }
    }
    
    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);

    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_len] = 0;
    printf("Read again: %s \n", buf);
    close(recv_sock);
    close(acpt_sock);
    return 0;
    // close(cln_sock);
}
void urg_handler(int signo){
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s \n", buf);
}
void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}

    