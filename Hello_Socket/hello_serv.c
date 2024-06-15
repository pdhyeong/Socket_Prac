#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message) {
	printf("%s \n",message);

}


int main(int argc,char *argv[]){
	int serv_sock;
	int cint_sock;


	struct sockaddr_in serv_addr;
	struct sockaddr_in cint_addr;
	socklen_t cin_addr_size;

	char message[] = "Hello World!";

	if(argc != 2) {
		printf("Usage : %s <Port>\n");
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	if(serv_sock == -1)
		error_handling("Socket() ERROR");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) 
		error_handling("bind() ERROR");

	if(listen(serv_sock,5) == -1)
		error_handling("listen ERROR");

	cin_addr_size = sizeof(cint_addr);
	cint_sock = accept(serv_sock,(struct sockaddr *)&cint_addr,&cin_addr_size);

	if(cint_sock == -1)
		error_handling("accpet ERROR");
	write(cint_sock,message,sizeof(message));
	close(cint_sock);
	close(serv_sock);

	return 0;
}
