#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define BUF_SIZE 1024
void error_handling(const char *message);
int main(int argc, char *argv[])
{
    //./calculate_client 127.0.0.1 1234
    if (argc != 3)
    {
        error_handling("args error!");
        exit(1);
    }

    int serv_sock;
    int ret;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        error_handling("sock() error");
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_family = AF_INET; // ipv4
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // connect
    ret = connect(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (-1 == ret)
    {
        error_handling("connect() error");
    }

    fputs("Connected.........\n", stdout);

    /*data transmission*/
    int operator_num;
    char operator;
    int num;
    long result;
    fputs("Operand count: ", stdout);
    scanf("%d", &operator_num);

    // transmission operator_num to server
    if (write(serv_sock, &operator_num, sizeof(operator_num)) == -1)
    {
        error_handling("write() error");
    }

    for (int i = 0; i < operator_num + 1; ++i)
    {
        if (i < operator_num)
        {
            printf("Operand %d: ", i+1);
            scanf("%d", &num);
            if (write(serv_sock, &num, sizeof(num)) == -1)
            {
                error_handling("write() error");
            }
        }
        else
        {
            fputs("Operator: ", stdout);
            scanf(" %c", &operator);
            if (write(serv_sock, &operator, 1) == -1)
            {
                error_handling("write() error");
            }
        }
    }
    
    if (read(serv_sock, &result, sizeof(long)) == -1)
    {
        error_handling("read() error");
    }
    printf("Operation result: %ld\n", result);
    close(serv_sock);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stdout);
}