#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message){
    printf("%s \n",message);
}
void read_chilproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1,&status,WNOHANG);
    printf("Removed proc id : %d \n",pid);
}

int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    act.sa_handler = read_chilproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD,&act, 0);
    
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 가운데 강제로 캐스팅 이유는 sockaddr 형식의 주소가 필요해서 이다.

    if(bind(serv_sock,(struct sockaddr *)&serv_adr,sizeof(serv_adr)) == -1)
        error_handling("Server Bind ERROR");
    
    if(listen(serv_sock,5) == - 1)
        error_handling("Listen ERROR");
    
    while(1){
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_adr, &adr_sz);
        if(clnt_sock == -1)
            error_handling("continue");
        else  puts("new Client Connected");

        pid = fork();
        if(pid == -1){
            close(clnt_sock);
            continue;
        }
        //자식 프로세스 영역
        if(pid == 0){
            close(serv_sock);
            while((str_len = read(clnt_sock,buf,BUF_SIZE)) != 0) write(clnt_sock,buf,str_len);
            close(clnt_sock);
            puts("Client disconnected\n");
            return 0;
        }
        else{
            close(clnt_sock);
        }
    }
    close(serv_sock);


    return 0;
}