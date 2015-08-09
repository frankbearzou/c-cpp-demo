#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAXLINE 1023

int main(int argc, char **argv)
{
	int sockfd;
	int n = 0;
	struct sockaddr_in connaddr;
	int port;
	pid_t pid;
	char send_buf[MAXLINE + 1] = { 0 };
	char recv_buf[MAXLINE + 1] = { 0 };

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("sockfdet error.");
		exit(-1);
	}

	bzero((void *)&connaddr, sizeof(connaddr));
	connaddr.sin_family = AF_INET;
	connaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (argc <= 1)
	{
		port = 7;
	}
	else
	{
		port = atoi(argv[1]);
	}
	printf("argc: %d, port: %d\n", argc, port);
	connaddr.sin_port = htons(port);
	
	if (connect(sockfd, (struct sockaddr *)&connaddr, sizeof(connaddr)) == -1)
	{
		perror("connect error");
		exit(-2);
	}

	pid = fork();
	// parent writes data to server
	if (pid > 0)
	{
		FILE *fp;
		if ((fp = fopen("test.c", "r")) == NULL)
		{
			perror("fopen error.");
			exit(0);
		}
		while (fgets(send_buf, MAXLINE, fp) != NULL)
		{
			write(sockfd, send_buf, strlen(send_buf));
		}
		fclose(fp);
	}
	// child reads data from server
	else if (pid == 0)
	{
		while ((n = read(sockfd, recv_buf, MAXLINE)) > 0)
		{
			recv_buf[n] = 0;
			printf("%s", recv_buf);
		}
	}
	else
	{
		perror("fork error.");
		exit(1);
	}

	if (waitpid(pid, NULL, 0) != pid)
	{
		perror("waitpid");
		exit(1);
	}

	return 0;
}
