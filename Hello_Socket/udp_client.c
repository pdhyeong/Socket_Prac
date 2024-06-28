#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void Error_handling(char *message)
{
	printf("%s \n",message);
}
int main(int argc,char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	struct sockaddr_in serv_adr, from_adr;
	socklen_t adr_sz;
	int str_len;

	if(argc != 3){
		printf("Uage %s IP <port>\n",argv[0]);
		exit(1);
	}

	sock = socket(PF_INET,SOCK_DGRAM,0);
	if(sock == -1)
		Error_handling("Socket() ERROR");

	memset(&serv_adr, 0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));


	// 보내고 받는형식의 Client 코드이므로 sendto부터
	while(1)
	{
		fputs("Insert Message(q to quit)",stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message , "q\n") || !strcmp(message, "quit\n"))
			break;
		sendto(sock,message,strlen(message),0,(struct sockaddr *) &serv_adr,sizeof(serv_adr));
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock,message, BUF_SIZE, 0, (struct sockaddr *)&from_adr,&adr_sz);
		message[str_len] = 0;
		printf("Message from server : %s\n",message);
	}
	close(sock);

	return 0;
}
