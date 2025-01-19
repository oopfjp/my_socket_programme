#include <43func.h>
#define TRUE 1
#define FALSE 0
int main(int argc, char* argv[]){
    int serv_sock, cln_sock;
    char message[30];
    int option, str_len;
    int ret;
    socklen_t optlen, clnt_adr_sz;
    struct sockaddr_in serv_addr, cln_addr;
    ARGS_CHECK(argc, 3);

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(serv_sock, -1, "socket()");

    
    optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
    

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    ERROR_CHECK(ret, -1, "bind()");

    ret = listen(serv_sock, 5);
    ERROR_CHECK(ret, -1, "listen()");

    clnt_adr_sz = sizeof(cln_addr);
    cln_sock = accept(serv_sock, (struct sockaddr*)&cln_addr, &clnt_adr_sz);

    //data transmission
    while((str_len = read(cln_sock, message, sizeof(message))) != 0){
        write(cln_sock, message,str_len);
        write(1, message, str_len);
    }
    return 0;
}