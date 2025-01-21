#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void errorHandling(const char* message);
int main(int argc, char* argv[]){

    if(argc!=3){
        errorHandling("args error!");
    }

    int sock;
    char buf[BUF_SIZE];
    int str_len;
    struct sockaddr_in sock_adr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        errorHandling("sock() error");
    }

    memset(&sock_adr, 0, sizeof(sock_adr));
    sock_adr.sin_family = AF_INET;
    sock_adr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&sock_adr, sizeof(sock_adr)) == -1){
        errorHandling("connect() error");
    }else{
        puts("connect success");
    }

    fd_set fds, cpfds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    FD_SET(0, &fds);
    
    
    while(1){
        cpfds = fds;
        select(sock+1, &cpfds, NULL, NULL, NULL);
        if(FD_ISSET(sock, &cpfds)){
            memset(buf, 0, sizeof(buf));
            if((str_len = read(sock, buf, BUF_SIZE)) != -1){
                buf[str_len] = 0;
                fputs(buf, stdout);
            }
        }else if(FD_ISSET(0, &fds)){
            memset(buf, 0, sizeof(buf));
            if((str_len = read(0, buf, sizeof(buf))) != -1){
                if(str_len == 0){
                    puts("Bye!");
                    break;
                }
                buf[str_len] = 0;
                write(sock, buf, str_len);
            }
        }
    }
    close(sock);
    return 0;
}


void errorHandling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
}