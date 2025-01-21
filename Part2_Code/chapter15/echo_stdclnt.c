#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#define BUF_SIZE 1024
#define ErrExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)
typedef struct sockaddr_in Addr_in;
typedef struct sockaddr Addr;

int main(int argc, char* argv[]){
    // .echo_clnt 127.0.0.1 1234

    if(3 != argc)
        ErrExit("args error");

    int sockfd = -1;
    Addr_in serv_adr;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
        ErrExit("socket");
    
    FILE* fp1;
    FILE* fp2;
    fp1 = fdopen(sockfd, "r+");
    fp2 = fdopen(0, "r");
    
    if(connect(sockfd, (Addr*)&serv_adr, sizeof(serv_adr)) == -1)
        ErrExit("connect");
    
    puts("connect success!");
    /*-------------------data transmission-------------------*/

    fd_set fds, cpfds;
    int str_Len;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    FD_SET(sockfd, &fds);
    char buf[BUF_SIZE];
    while(1){
        cpfds = fds;
        select(sockfd+1, &cpfds, NULL, NULL, NULL);
        if(FD_ISSET(0, &cpfds)){
            memset(buf, 0, sizeof(buf));
            // str_Len = read(0, buf, sizeof(buf) - 1);
            // fgets(buf, BUF_SIZE - 1, fp2);
            str_Len = fread(buf, 1, BUF_SIZE - 1, fp2);
            buf[str_Len] = 0;
            // write(sockfd, buf, str_Len);
            fputs(buf, fp1);
        }
        if(FD_ISSET(sockfd, &cpfds)){
            memset(buf, 0, sizeof(buf));
            // str_Len = read(sockfd, buf, sizeof(buf));
            // fgets(buf, BUF_SIZE - 1, fp1);
            str_Len = fread(buf, 1, BUF_SIZE - 1, fp1);
            buf[str_Len] = 0;
            fputs(buf, stdout);
        }
    }
    fclose(fp1);
    fclose(fp2);
    close(sockfd);
    return 0;
}