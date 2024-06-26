#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char *message){
        printf("%s \n",message);
}
int calculate(int opnum,int opinfo[], char op)
{
	int result = opinfo[0], i;
	switch(op)
	{
		case '+':
			for(i = 1;i<opnum;i++) 
				result += opinfo[i];
			break;
		case '-':
			for(i = 1;i<opnum;i++)
				result -= opinfo[i];
			break;
		case '*':
			for(i = 1;i<opnum;i++)
				result *= opinfo[i];
			break;
	}
	return result;
}
int main(int argc, char *argv[]) {
        int serv_sock,cint_sock;
        char opinfo[BUF_SIZE];
	int result, opand_cnt,i;
	int recv_cnt,recv_len;

        struct sockaddr_in serv_addr;
        struct sockaddr_in cint_addr;
	int want_cnt;
        socklen_t cint_adr_sz;

        if(argc != 2) {
                printf("Usage : %s <port>\n",argv[0]);
                exit(1);
        }

        serv_sock = socket(PF_INET,SOCK_STREAM,0);
        if(serv_sock == -1)
                error_handling("socket() Error");

        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(atoi(argv[1]));

        if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
                error_handling("bind() ERROR");

        if(listen(serv_sock, 5) == -1)
                error_handling("listen ERROR");

        cint_adr_sz = sizeof(cint_addr);
	printf("Put in the Number of Time you want\n");
	scanf("%d",&want_cnt);
        for(i = 0;i<want_cnt;i++)
	{

		printf("now Connection Number = %d\n",i+1);
		opand_cnt = 0;
		recv_len = 0;
                cint_sock = accept(serv_sock, (struct sockaddr *)&cint_addr,&cint_adr_sz);
		if(cint_sock != -1)
			printf("%d Number Socket Accpect\n",i+1);
		read(cint_sock,&opand_cnt,1);
	
		while(recv_len < (opand_cnt * OPSZ + 1))
		{
			recv_cnt = read(cint_sock,&opinfo[recv_len],BUF_SIZE-1);
			recv_len += recv_cnt;
		}
		result = calculate(opand_cnt,(int *)opinfo, opinfo[recv_len-1]);
		write(cint_sock, (char *)&result, sizeof(result));
		printf("now Client op Result = %d\n",result);
		printf("Number %d Socket closed\n\n",i+1);
		fflush(stdout);
		
		close(cint_sock);
        }
        close(serv_sock);

        return 0;
}
