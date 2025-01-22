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

    int sock;
    char buf[BUF_SIZE];
    Addr_in serv_addr;

    FILE* readfp;
    FILE* writefp;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sock, (Addr*)&serv_addr, sizeof(serv_addr));
    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    while (1)
    {
        if(fgets(buf, sizeof(buf), readfp) == NULL)
            break;
        fputs(buf, stdout);
        fflush(stdout);
    }

    fputs("FROM CLINET: Thank you! \n", writefp);
    fflush(writefp);
    fclose(writefp);fclose(readfp);
    return 0;

}