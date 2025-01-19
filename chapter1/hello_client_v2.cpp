#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char *argv[]){
    if(3 != argc){
        cout << "args error!" << endl;
    }

    // step1
    int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    cout << "sock_fd = " << sock_fd << endl;

    if(sock_fd < 0){
        cout << "sock error!" << endl;
    }

    // step2
    
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = PF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // step3
    if(connect(sock_fd, (struct sockaddr*)&serAddr, sizeof(serAddr)) == -1){
        cout << "connect error" << endl;
    }
    
    puts("connect success!");
    
    char buf[1024] = {0};
    // string buf;
    // sleep(1000);
    for(int i = 0; i < 3000; ++i){
        cout << "Wait time: " << i << endl;
    }
    if(recv(sock_fd, buf,  sizeof(buf) - 1 , 0) == -1){
        cout << "recv error!" << endl;
    }

    cout << buf << endl;
    
    close(sock_fd);
}
    
