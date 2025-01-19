#include <stdio.h>
#include <sys/signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
void sig_process(int sig);
void timeout(int sig);
int main(int argc, char* argv[]){

    
    struct sigaction act, act2;
    act.sa_handler = sig_process;
    act2.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    sigemptyset(&act2.sa_mask);
    act.sa_flags = 0;
    act2.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    sigaction(SIGALRM, &act2, 0);
    while(1){

    }
    return 0;
}

void sig_process(int sig){
    putc('\n', stdout);
    printf("quit ready?, please enter Y \n");
    char bu;
    alarm(1);
    while(scanf(" %c",&bu)){
        if(bu == 'Y'){
            exit(1);
        }
        // getchar();
       
    } 
}

void timeout(int sig){
    if(sig == SIGALRM){
        puts("quit ready?, please enter Y");
    }
    alarm(1);
}