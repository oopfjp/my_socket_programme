#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 100
void error_handling(const char* message);


int main(){
    int fd;
    char buf[BUF_SIZE];
    fd = open("data.txt", O_RDONLY);
    if(-1 == fd){
        error_handling("open() error!");
    }
    printf("file descriptor: %d\n", fd);

    if(read(fd, buf, sizeof(buf)) == -1){
        error_handling("write() error!");
    }
    printf("file data: %s", buf);

    
    int write_fd = open("copy.txt", O_CREAT | O_TRUNC | O_WRONLY);
    printf("file descriptor: %d\n", write_fd);
    if(-1 == write_fd){
        error_handling("open() error!");
    }
    if(write(write_fd, buf, strlen(buf)) == -1){
        error_handling("copy error!\n");
    }
    
    close(fd);
    close(write_fd);
    return 0;
}


void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}