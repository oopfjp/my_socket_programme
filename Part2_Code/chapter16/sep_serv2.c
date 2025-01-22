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
        ErrExit("args");

int serv_sock, clnt_sock;
Addr_in serv_adr, clnt_adr;
FILE* readfp;
FILE* writefp;
socklen_t clnt_adr_sz;
char buf[BUF_SIZE]={0,};
    clnt_adr_sz = sizeof(clnt_adr);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == serv_sock)
        ErrExit("socket");

    //set serv_sock
int opt;
    opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

    if(bind(serv_sock, (Addr*)&serv_adr, sizeof(serv_adr)) == -1)
        ErrExit("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        ErrExit("listen() error");
    
    clnt_sock = accept(serv_sock, (Addr*)&clnt_adr, &clnt_adr_sz);
    if(-1 == clnt_sock)
        ErrExit("accept() error");

    puts("connect success!");
    /*-------------------data transmission-------------------*/
    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(dup(clnt_sock), "w");

    fputs("FROM SERVER: Hi~ client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);


    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp); fputs(buf, stdout);
    fclose(readfp);
    return 0;

}