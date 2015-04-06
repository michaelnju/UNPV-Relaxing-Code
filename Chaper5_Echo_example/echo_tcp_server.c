//#include	"unp.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#define LISTENQ 5

#define MAXLINE 2048
#define SA	struct sockadddr
#define SERV_PORT 9877


void str_echo(int sockfd);
int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		//connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
		connfd = accept(listenfd,(SA*)NULL, NULL);
		printf("Successfully Connected!\n");
		if ( (childpid = fork()) == 0) {	/* child process */
			close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		close(connfd);			/* parent closes connected socket */
	}
}

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
	{

			printf("write back to the client!\n");
			write(sockfd, buf, n);
			//printf("write back to the client!");
	}
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
	{
		perror("str_echo: read error");
		exit(1);
	}
}
