#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[]){
    int recv_sock;
    struct sockaddr_in recv_adr;
    char buf[BUF_SIZE];
    int str_len;

    struct ip_mreq join_adr;

    if(argc != 3){
        printf("Usage : %s <GroupIP> <Port> \n",argv[0]);
        exit(1);
    }

    recv_sock = socket(PF_INET,SOCK_DGRAM,0);
    memset(&recv_adr,0,sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[2]));
    
    if(bind(recv_sock,(struct sockaddr *)&recv_adr,sizeof(recv_adr)) == -1){
        error_handling("bind ERROR()");
    }

    while(1){
        str_len = recvfrom(recv_sock,buf,BUF_SIZE-1,0,NULL,0);
        if(str_len < 0){
            break;
        }
        buf[str_len] = 0;
        fputs(buf,stdout);
    }
    close(recv_sock);
    return 0;
}