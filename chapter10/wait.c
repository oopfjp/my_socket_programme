#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

/*wait()函数会阻塞*/
int main(int argc, char* argv[]){

    int status;
    pid_t pid = fork();

    if(pid == 0){
        //子进程
        return 3;
    }
    else{
        //父进程
        printf("Child PID: %d \n", pid);
        pid = fork();
        if(pid == 0){
            exit(7);
        }
        else{
            printf("Child PID: %d \n", pid);
            wait(&status);
            if(WIFEXITED(status)){
                printf("Child send one : %d \n", WEXITSTATUS(status));
            }
            wait(&status);
            if(WIFEXITED(status)){
                printf("Child send one : %d \n", WEXITSTATUS(status));
            }
            sleep(30);
        }   
    }
    return 0;
}