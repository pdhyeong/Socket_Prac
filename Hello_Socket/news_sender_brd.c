#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[]){
    int send_sock;
    struct sockaddr_in broad_adr;
    int time_live = TTL;
    FILE *fp;
    int so_brd = 1;
    char buf[BUF_SIZE];

    if(argc != 3){
        printf("Usage : %s <GroupIP> <Port> \n",argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET,SOCK_DGRAM,0);
    memset(&broad_adr,0,sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock,SOL_SOCKET,SO_BROADCAST,(void *)&so_brd,sizeof(so_brd));
    if((fp = fopen("news.txt","r"))== NULL){
        error_handling("fopen() error");
    }

    while(!feof(fp)){
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock,buf,strlen(buf),0,(struct sockaddr*)&broad_adr,sizeof(broad_adr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}