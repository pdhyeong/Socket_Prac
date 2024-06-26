#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message){
        printf("%s \n",message);
}

int main(int argc,char *argv[]) {
        int sock;
        char opmsg[BUF_SIZE];
	int result, opand_cnt ,i;;

        struct sockaddr_in serv_addr;

        if(argc != 3) {
                printf("Usage : %s <port>\n",argv[0]);
                exit(1);
        }

        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == -1)
                error_handling("socket() ERROR");

        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(atoi(argv[2]));

        if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
                error_handling("connect() ERROR");

        else
                puts("Connected.........");


	fputs("Operand Count : ",stdout);
	scanf("%d",&opand_cnt);

	opmsg[0] = (char) opand_cnt;

	for(i = 0;i<opand_cnt;i++)
	{
		printf("Operator : %d\n",i+1);
		scanf("%d",(int *) &opmsg[i * OPSZ + 1]);
	}
	fgetc(stdin);
	fputs("Operator : ",stdout);
	scanf("%c",&opmsg[opand_cnt * OPSZ + 1]);
	write(sock, opmsg,opand_cnt * OPSZ + 2);
	read(sock, &result, RLT_SIZE);
	printf("Result = %d\n",result);
	close(sock);

        return 0;
}
