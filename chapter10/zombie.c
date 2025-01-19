#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
    
    pid_t pid = fork();

    if(pid == 0){
        puts("Hi, I am a child Process");
        return 9;
        
    }else{
        // printf("Child Process Id: %d\n", pid);
        if(fork() == 0){
           _exit(10); 
        }else{
            wait(NULL);
            // sleep(20);
        }
        wait(NULL);
        sleep(10);
    }
    // if(pid == 0){
    //     puts("Hi, I am a child Process");
    // }else{
    //     printf("Child Process Id: %d\n", pid);
    //     sleep(30);
    // }

    // if(pid == 0){
    //     puts("End child process");
    // }else{

    //     puts("End parent process");
    // }
    return 0;
}