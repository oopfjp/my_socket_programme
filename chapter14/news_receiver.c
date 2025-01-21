#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]){
    int recv_sock;
    int str_len;
    struct sockaddr_in adr;
    struct ip_mreq join_adr;
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Group IP> <Port>\n", argv[0]);
        exit(1);
    }

    recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (recv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[2]));

    if (bind(recv_sock, (struct sockaddr*)&adr, sizeof(adr)) == -1) {
        perror("bind() error");
        exit(1);
    }

    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr)) == -1) {
        perror("setsockopt() error");
        exit(1);
    }

    while (1) {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0) {
            perror("recvfrom() error");
            break;
        }
        buf[str_len] = '\0'; // 确保字符串正确终止
        if (str_len > 0 && buf[str_len - 1] == '\n') {
            buf[str_len - 1] = '\0'; // 去除可能存在的换行符
        }
        fputs(buf, stdout);
    }

    close(recv_sock);
    return 0;
}