#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

    FILE* fp;
    int fd = open("data.dat", O_CREAT | O_WRONLY | O_TRUNC);
    if(-1 == fd){
        fputs("file open error", stdout);
        return -1;
    }

    printf("First file descriptor: %d\n", fd);
    fp = fdopen(fd, "w");
    fputs("TCP/IP SOCKET RPOGRAMMING \n", fp);
    printf("Second file descriptor: %d \n", fileno(fp));
    fclose(fp);
    return 0;
}