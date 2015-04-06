//#include	"unp.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LISTENQ 5
#define MAXLINE 2048
#define SERV_PORT 9877

typedef struct sockaddr SA;

void str_cli(FILE *fp, int sockfd);
int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
	{
		perror("usage: tcpcli <IPaddress>");
		exit(-1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("Connect Error!");
		exit(1);
	}
	else
			printf("Connected Successfully!\n");

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

void
str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		write(sockfd, sendline, strlen(sendline));

		if (read(sockfd, recvline, MAXLINE) == 0)
		{
			perror("str_cli: server terminated prematurely");
			exit(-1);
		}
		fputs(recvline, stdout);
	}
}
