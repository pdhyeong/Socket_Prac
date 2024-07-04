#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message){
    printf("%s \n",message);
}

int main(int argc, char *argv[]){
    int sock;
    int snd_buf = 1024 * 3,rcv_buf = 1024 * 3;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    state = setsockopt(sock,SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, sizeof(rcv_buf));
    if(state) error_handling("setSocketopt ERROR");
    state = setsockopt(sock,SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, sizeof(snd_buf));
    if(state) error_handling("setSocketopt ERROR");

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf,&len);
    if(state) error_handling("setSocketopt ERROR");

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf,&len);
    if(state) error_handling("setSocketopt ERROR");

    printf("Input Buffer size : %d \n",rcv_buf);
    printf("Output Buffer size : %d \n",snd_buf);


    return 0;
}