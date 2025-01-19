#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
void error_handling(const char* message);


int main(){
    int fd;
    char buf[] = "Let's go Worries!\n";
    fd = open("data.txt", O_WRONLY|O_APPEND);
    if(-1 == fd){
        error_handling("open() error!");
    }
    printf("file descriptor: %d\n", fd);

    int step = 3;
    while(step--){
        if(write(fd, buf, strlen(buf)) == -1){
            error_handling("write() error!");
        }
    }

    int read_fd = open("data.txt", O_RDONLY);
    char message[4096] ={0};

    while(read(read_fd, message, sizeof(message))){
       printf("%s\n", message);
       memset(message, 0, sizeof(message));
    }
    
    close(fd);
    close(read_fd);
    return 0;
}


void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}