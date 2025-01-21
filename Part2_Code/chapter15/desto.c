#include <stdio.h>
#include <fcntl.h>
#define BUF_SIZE 3

int main(int argc, char* argv[]){
    
    FILE* fp1;
    int fd = open("file.bin", O_RDONLY);
    if(-1 == fd){
        fputs("file open error", stdout);
        return -1;
    }
    fp1 = fdopen(fd, "r");
    // fputs("NetWork C Programming \n", fp1);
    char buf[BUF_SIZE];

    FILE* fp2 = fopen("cpy.bin", "w");
    while (fgets(buf, BUF_SIZE, fp1)!=NULL)
    {
        fputs(buf, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}