#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
void error_handling(char* message);

int main(int argc, char* argv[]){
    char* addr = "127.28.221.10";
    struct sockaddr_in addr_inet;
    if(!inet_aton(addr, &addr_inet.sin_addr)){
        error_handling("Conversion error");
    }
    else{
        printf("Network ordered integer addr: %#x \n", 
                        addr_inet.sin_addr.s_addr);
    }
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}