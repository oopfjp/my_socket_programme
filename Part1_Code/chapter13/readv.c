#include <sys/uio.h>
#include <stdio.h>

int main(int argc, char* argv[]){

    struct iovec vec[2];
    char buf1[BUFSIZ] = {0, };
    char buf2[BUFSIZ] = {0, };
    int str_len;

    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUFSIZ;

    str_len = readv(0, vec, 2);
    printf("Reads bytes: %d \n", str_len);
    printf("First message: %s \n", buf1);
    printf("Second message: %s \n", buf2);
    return 0;
}