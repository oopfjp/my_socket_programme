#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 1024
int main(int argc, char* argv[]){
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in sock_adr;
    
    memset(&sock_adr, 0, sizeof(sock_adr));
    sock_adr.sin_port = htons(atoi(argv[2]));
    sock_adr.sin_family = AF_INET;
    sock_adr.sin_addr.s_addr = inet_addr(argv[1]);
    char buf[BUF_SIZE];
    socklen_t adr_sz = sizeof(sock_adr);
    sendto(sock, "Hello World!\n", 14, 0, (struct sockaddr*)&sock_adr, sizeof(sock_adr));
    sendto(sock, "Hello World!\n", 14, 0, (struct sockaddr*)&sock_adr, sizeof(sock_adr));
    sendto(sock, "Hello World!\n", 14, 0, (struct sockaddr*)&sock_adr, sizeof(sock_adr));

    recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr*)&sock_adr, &adr_sz);
    puts(buf);
}