#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 100
void error_handling(const char* message);


int main(){

    FILE* source_file = fopen("data.txt","r");
    FILE* target_file = fopen("copy1","a");
    char buf[1024]={0};
    while(read(source_file->_fileno, buf, sizeof(buf))){
        if(write(target_file->_fileno, buf, strlen(buf) + 1) == -1){
            error_handling("write() error!\n");
        }
        memset(buf, 0, sizeof(buf));
    }

    // printf("%d\n", source_file->_fileno);
    // printf("%d\n", target_file->_fileno);
    // fwrite()
    
    return 0;
}


void error_handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    _exit(1);
}