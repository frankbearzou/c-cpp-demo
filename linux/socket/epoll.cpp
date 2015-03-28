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
#include <sys/epoll.h>
#include <poll.h>
#include <errno.h>

#include <iostream>
#include <cctype>
#include <algorithm>
#include <iterator>

using namespace std;

const uint16_t SERVER_PORT = 8000;
#define MAX_EVENTS 65535


int main()
{
	int sockfd;
	sockaddr_in serveraddr;
	int yes = 1;

	// socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("socket");
		exit(-1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes)) == -1) {
		perror("setsockopt");
	}

	// bind
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);

	if (bind(sockfd, (sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
		perror("bind");
		exit(-1);
	}

	// listen
	if (listen(sockfd, 128) == -1) {
		perror("listen");
		exit(-1);
	}

	// epoll
	int epfd;
	epoll_event tempevent;
	epoll_event epevent[65536];
	//int maxevents = 0;
	int nready = 0;

	// create epoll
	epfd = epoll_create(1);
	if (epfd == -1) {
		perror("epoll_create");
		exit(-1);
	}

	// init with sockfd
	memset(&epevent, 0, sizeof(epevent));
	memset(&tempevent, 0, sizeof(tempevent));
	tempevent.events = EPOLLIN;
	tempevent.data.fd = sockfd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &tempevent) == -1) {
		perror("epoll_ctl");
	}
	//++maxevents;

	// epoll loop
	while (1)
	{
		nready = epoll_wait(epfd, epevent, MAX_EVENTS, -1);
		if (nready == -1) {
			if (errno == EINTR) {
				continue;
			}
			perror("epoll_wait");
		}
		// deal with nready times events
		for (int i = 0; i < nready; ++i)
		{
			// read events
			if (epevent[i].events & EPOLLIN)
			{
				// new connection comes
				if (epevent[i].data.fd == sockfd)
				{
					int connfd;
					sockaddr_in connaddr;
					socklen_t connaddrlen;

					// accept
					memset(&connaddr, 0, sizeof(connaddr));
					connaddrlen = sizeof(connaddr);

					connfd = accept(sockfd, (sockaddr *)&connaddr, &connaddrlen);
					if (connfd == -1) {
						perror("accept");
					}
					// epoll_ctl
					memset(&tempevent, 0, sizeof(tempevent));
					tempevent.events = EPOLLIN;
					tempevent.data.fd = connfd;
					if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &tempevent) == -1) {
						perror("epoll_ctl");
					}

					// show new connction information
					char buf_addr[128] = { 0 };
					cout << "welcome:"
							<< inet_ntop(AF_INET, (void *)&connaddr.sin_addr.s_addr, buf_addr, connaddrlen)
							<< ":"
							<< ntohs(connaddr.sin_port)
							<< endl;
				}
				else     // deal with old connection read
				{
					int connfd;
					char buf[4096] = { 0 };
					int nbytes;

					connfd = epevent[i].data.fd;
					// read data
					nbytes = recv(connfd, buf, sizeof(buf), 0);
					if (nbytes == -1) {
						perror("recv");
					}
					else if (nbytes == 0)   // close connection
					{
						if (epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL) == -1) {
							perror("epoll_ctl");
						}
						close(connfd);
					}
					else     // deal with data
					{
						transform(begin(buf), end(buf), begin(buf),
								[](const char c){return toupper(c);});
						send(connfd, (void *)buf, strlen(buf), 0);
					}
				}  // end of deal with old connection read
			}  // end of read events
			else
			{
				cout << "events: " << epevent[i].events
						<< "fd: " << epevent[i].data.fd
						<< endl;
			}
		}   // end of deal with nready times events
	}   // end of epoll loop

	return 0;
}



