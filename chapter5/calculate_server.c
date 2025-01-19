#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUF_SIZE 1024
void error_handling(const char *message);
int main(int argc, char *argv[])
{
    if (3 != argc)
    {
        error_handling("args error!");
        exit(1);
    }

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clntaddr_size;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0); // TCP
    if (-1 == serv_sock)
    {
        error_handling("sock() error");
    }

    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_family = AF_INET; // ipv4
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    // bind
    int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (-1 == ret)
    {
        error_handling("bind() error");
    }

    // listen
    ret = listen(serv_sock, 10);
    if (-1 == ret)
    {
        error_handling("listen() error");
    }

    // accept
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clntaddr_size);
    if (-1 == clnt_sock)
    {
        error_handling("accept() error");
    }
    fputs("connect success!\n", stdout);

    int operator_num;
    if (read(clnt_sock, &operator_num, sizeof(operator_num)) == -1)
    {
        error_handling("read() error");
    }

    printf("operator_num = %d\n", operator_num);
    char operator;
    int *operators[operator_num];
    long result;

    for (int i = 0; i < operator_num; ++i)
    {
        operators[i] = (int *)malloc(sizeof(int));
        if (read(clnt_sock, operators[i], sizeof(int)) == -1)
        {
            error_handling("read() error");
        }
    }

    if (read(clnt_sock, &operator, 1) == -1)
    {
        error_handling("read() error");
    }

    switch (operator)
    {
    case '-':
        result = *operators[0];
        for (int i = 1; i < operator_num; ++i)
        {
            result -= *operators[i];
        }
        break;
    case '+':
        for (int i = 0; i < operator_num; ++i)
        {
            result += *operators[i];
        }
        break;
    case '*':
        result = *operators[0];
        for (int i = 1; i < operator_num; ++i)
        {
            result *= *operators[i];
        }
        break;
    default:
        break;
    }

    if (write(clnt_sock, &result, sizeof(long)) == -1)
    {
        error_handling("write() error");
    }

    for(int i = 0; i < operator_num; ++i){
        free(operators[i]);
    }
    close(serv_sock);
    close(clnt_sock);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stdout);
}