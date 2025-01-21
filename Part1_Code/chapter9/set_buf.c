#include <43func.h>
int main(int argc, char* argv[]){

    int sock;
    int snd_buf = 1024*3, rcv_buf = 1024*3, state;
    socklen_t len;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state){
        ERROR_CHECK(state, -1, "setsockopt() error");
    }

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if(state){
        ERROR_CHECK(state, -1, "setsockopt() error");
    }

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state){
        ERROR_CHECK(state, -1, "getsockopt() error");
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if(state){
        ERROR_CHECK(state, -1, "getsockopt() error");
    }

    printf("Input buffer size: %d \n", rcv_buf);  
    printf("Output buffer size: %d \n", snd_buf);  
    return 0;
}