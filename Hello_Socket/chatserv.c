#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLIENT 256

int clnt_cnt = 0;
int clnt_socks[MAX_CLIENT];
pthread_mutex_t mutex;

void send_message(char *message,int len){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0;i<clnt_cnt;i++){
        write(clnt_socks[i],message,len);
    }
    pthread_mutex_unlock(&mutex);
}
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

void *handle_clinet(void *arg){
    int clnt_sock = *((int *)arg);
    int str_len = 0,i;
    char message[BUF_SIZE];

    while((str_len = read(clnt_sock,message,sizeof(message))) != 0){
        send_message(message,str_len);
    }
    pthread_mutex_lock(&mutex);
    for(i = 0;i<clnt_cnt;i++){
        if(clnt_sock == clnt_socks[i]){
            while(i++<clnt_cnt-1){
                clnt_socks[i] = clnt_socks[i+1];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    return NULL;
}

int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    int str_len;
    int clnt_adr_size;
    pthread_t t_id;
    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    pthread_mutex_init(&mutex,NULL);

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr *)&serv_adr,sizeof(serv_adr)) == -1){
        error_handling("bind ERROR");
    }
    if(listen(serv_sock,5) == -1){
        error_handling("listen ERROR");
    }

    while(1){
        clnt_adr_size = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_adr,&clnt_adr_size);
        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id,NULL,handle_clinet,(void *)&clnt_sock);
        pthread_detach(t_id);
        printf("Connected clinet IP : %s \n",inet_ntoa(clnt_adr.sin_addr));
    }
    close(serv_sock);
    return 0;
}
