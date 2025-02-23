#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
using std::cout;
using std::endl;

void error_handling(const char* message);
int main(int argc, char* argv[]){

    int i;
    struct hostent* host;
    if(argc!=2){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    host = gethostbyname(argv[1]);
    if(!host){
        error_handling("gethost... error");
    }
    cout << "Official name: " << host->h_name << endl;

    for(i = 0; host->h_aliases[i]; ++i){
        cout << "Aliases " << i+1 << ": " << host->h_aliases[i] << endl; 
    }

    cout << "Address type: " << ((host->h_addrtype == AF_INET)?"AF_INET":"AF_INET6") << endl;
    for(i = 0; host->h_addr_list[i]; ++i){
        cout << "IP addr " << i+1 << ": " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << endl;
    }
    return 0;
}

void error_handling(const char* message){
    fputs(message, stdout);
    fputc('\n', stdout);
    exit(1);
}