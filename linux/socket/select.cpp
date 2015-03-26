#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>

#include <cctype>
#include <algorithm>
#include <iterator>

using namespace std;

const uint16_t SERVER_PORT = 8000;

int main()
{
	int listenfd;
	sockaddr_in listenaddr;
	int yes;
	// fd
	fd_set all_selectfd;
	fd_set read_selectfd;
	int max_selectfd;

	int nready;

	// 1. socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		perror("secket");
		return -1;
	}

	// 2. bind
	memset(&listenaddr, 0, sizeof(listenaddr));
	listenaddr.sin_family = AF_INET;
	listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenaddr.sin_port = htons(SERVER_PORT);

	yes = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes)) == -1)
	{
		perror("setsockopt");
		exit(-1);
	}

	if (bind(listenfd, (sockaddr *)&listenaddr, sizeof(listenaddr)) == -1)
	{
		perror("bind");
		exit(-1);
	}

	// 3. listen
	if(listen(listenfd, 128) == -1)
	{
		perror("listen");
		exit(-1);
	}

	// 4. select
	FD_ZERO(&all_selectfd);
	FD_SET(listenfd, &all_selectfd);
	FD_ZERO(&read_selectfd);
	max_selectfd = listenfd;

	while (1)
	{
		read_selectfd = all_selectfd;
		nready = select(max_selectfd + 1, &read_selectfd, NULL, NULL, NULL);
		if (nready == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				perror("select");
				exit(-1);
			}
		}
		// accept
		if(FD_ISSET(listenfd, &read_selectfd))
		{
			int connfd;
			sockaddr_in connaddr;
			socklen_t connaddrlen;

			memset(&connaddr, 0, sizeof(connaddr));
			connaddrlen = sizeof(connaddr);
			connfd = accept(listenfd, (sockaddr *)&connaddr, &connaddrlen);
			if (connfd == -1)
			{
				perror("accept");
				continue;
			}

			// set fds
			FD_SET(connfd, &all_selectfd);
			if (connfd > max_selectfd)
			{
				max_selectfd = connfd;
			}

			if (--nready == 0)
				continue;
		} // end of accept

		// old fd
		for (int i = 0; i <= max_selectfd; ++i)
		{
			if (FD_ISSET(i, &read_selectfd))
			{
				int connfd = i;
				char buf[4096] = { 0 };
				int readn = 0;

				readn = recv(connfd, buf, sizeof(buf), 0);
				// client closed
				if (readn == 0)
				{
					close(connfd);
					FD_CLR(connfd, &all_selectfd);
					continue;
				}
				// write
				for_each(begin(buf), end(buf), [&](char &c){c=toupper(c);});
				send(connfd, buf, readn, 0);

				if (--nready == 0)
				{
					break;
				}
			}
		}
	}


	return 0;
}
