//#include	"unp.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 4096
#define PORT 13
typedef struct sockaddr SA;
int
main(int argc, char **argv)
{
	int					sockfd, n, counter = 0;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
	{
		perror("usage: a.out <IPaddress>");
		exit(1);
	}
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}	
	else
	{
		printf("socket created successfully!\n");
		printf("socket id is: %d\n",sockfd);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(PORT);	/* daytime server */
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect error");
		exit(1);		
	}

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		counter++;
		recvline[n] = 0;	/* null terminate */
		fputs(recvline, stdout);
	}
	if (n < 0)
	{ 
		perror("read error");
		exit(1);
	
	}
	printf("counter = %d\n", counter);
	exit(0);
}
