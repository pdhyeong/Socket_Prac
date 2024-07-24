#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc,char *argv[]){
    int sock;
    char buf[BUF_SIZE];
    struct sockaddr_in clnt_adr;
    
    FILE * readfp;
    FILE * writefp;

    sock = socket(PF_INET,SOCK_STREAM,0);
    memset(&clnt_adr,0,sizeof(clnt_adr));
    clnt_adr.sin_family = AF_INET;
    clnt_adr.sin_addr.s_addr = inet_addr(argv[1]);
    clnt_adr.sin_port = htons(atoi(argv[2]));

    connect(sock,(struct sockaddr *)&clnt_adr,sizeof(clnt_adr));

    readfp = fdopen(sock,"r");
    writefp = fdopen(sock,"w");

    while(1){
        if(fgets(buf,sizeof(buf),readfp) == NULL){
            break;
        }
        fputs(buf, stdout);
        fflush(stdout);
    }

    fputs("FROM CLIENT : Thank you! \n",writefp);
    fflush(writefp);
    fclose(writefp); fclose(readfp);
    
    return 0;
}