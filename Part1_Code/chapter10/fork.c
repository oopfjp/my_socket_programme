#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
int gval = 10;
int main()
{
    int lval = 20;
    lval += 5;
    gval++;
    pid_t pid = fork();
    if (pid == 0)
    {
        gval += 2, lval += 2;
    }else{
        gval -= 2, lval -= 2;
    }

    if(pid == 0){
        printf("Child proc: [%d, %d] \n", gval, lval);
    }else{
        printf("Parent proc: [%d, %d] \n", gval, lval);
    }
    
}