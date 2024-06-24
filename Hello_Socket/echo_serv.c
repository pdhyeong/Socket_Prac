#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message){
	printf("%s \n",message);
}

int main(int argc, char *argv[]) {
	int serv_sock,cint_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_addr;
	struct sockaddr_in cint_addr;
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

	for(i = 0;i<5;i++)
	{
		cint_sock = accept(serv_sock, (struct sockaddr *)&cint_addr,&cint_adr_sz);
		if(cint_sock == -1)
			error_handling("accpet() ERROR");
		else
			printf("Connect() Success client %d\n",i+1);
		while((str_len=read(cint_sock,message , BUF_SIZE)) != 0)
			write(cint_sock,message,str_len);
		close(cint_sock);
	}
	close(serv_sock);

	return 0;
}
