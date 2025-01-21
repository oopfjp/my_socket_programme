#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define TTL 64
#define BUF_SIZE 30

int main(int argc, char* argv[]){
    int send_sock;
    struct sockaddr_in mul_adr;
    int time_live = TTL;
    FILE* fp;
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Multicast IP> <Port>\n", argv[0]);
        exit(1);
    }

    send_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    if (setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live)) == -1) {
        perror("setsockopt() error");
        exit(1);
    }

    if ((fp = fopen("../chapter1/data.txt", "r")) == NULL) {
        perror("fopen() error");
        exit(1);
    }

    while (fgets(buf, BUF_SIZE, fp) != NULL) {
        if (sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr)) == -1) {
            perror("sendto() error");
            exit(1);
        }
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
    return 0;
}