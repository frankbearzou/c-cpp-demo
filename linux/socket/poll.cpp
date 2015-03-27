/*
This is a simple poll server
*/
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
#include <poll.h>
#include <errno.h>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <iterator>

using namespace std;

const uint16_t SERVER_PORT = 8000;
#define MAX_POLL_SIZE 1024

int main()
{
	int sockfd;
	sockaddr_in serveraddr;
	int yes = 1;

	// 1. socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(-1);
	}
	// reuse address
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes)) == -1) {
		perror("setsockopt");
	}

	// 2. bind
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);

	bind(sockfd, (sockaddr *)&serveraddr, sizeof(serveraddr));

	// 3. listen
	listen(sockfd, 128);

	// 4. poll
	pollfd fds[MAX_POLL_SIZE];
	nfds_t max_nfds = 0;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	for (int i = 1; i < MAX_POLL_SIZE; ++i) {
		fds[i].fd = -1;
	}

	int nready = 0;
	// poll loop
	while (1)
	{
		nready = poll(fds, max_nfds + 1, -1);
		if (nready == -1) {
			perror("poll");
			exit(-1);
		}
		// new connections arrive
		if (fds[0].revents & POLLIN)
		{
			sockaddr_in connaddr;
			memset(&connaddr, 0, sizeof(connaddr));
			socklen_t connaddrlen = sizeof(connaddr);
			char buf[4096] = { 0 };

			// 5. accept
			int connfd = accept(sockfd, (sockaddr *)&connaddr, &connaddrlen);
			if(connfd == -1) {
				perror("accept");
				continue;
			}
			// insert into pollfd structure fds
			bool is_inserted = false;
			int i;
			for (i = 1; i < MAX_POLL_SIZE; ++i) {
				if (fds[i].fd == -1) {
					fds[i].fd = connfd;
					fds[i].events = POLLIN|POLLERR;
					is_inserted = true;
					break;
				}
			}
			if(!is_inserted) {
				cout << "poll is full" << endl;
			}
			// increase monitored number
			if ((unsigned int)i > max_nfds) {
				++max_nfds;
			}

			// print client ip
			inet_ntop(AF_INET, (void *)&connaddr.sin_addr.s_addr, buf, sizeof(connaddr));
			cout << "welcome: " << buf;
			memset(buf, 0, sizeof(buf));

			cout << ":" << ntohs(connaddr.sin_port) << endl;

			if (--nready == 0) {
				continue;
			}
		}
		// old connections read
		for (int i = 1; i < MAX_POLL_SIZE; ++i)
		{
			if (fds[i].fd == -1) {
				continue;
			}
			if (fds[i].revents & (POLLIN|POLLERR)) {
				int connfd = fds[i].fd;
				int nbytes = 0;
				char buf[4096] = { 0 };

				nbytes = recv(connfd, buf, sizeof(buf), 0);
				// deal with error
				if(nbytes <= 0) {
					close(connfd);
					fds[i].fd = -1;
					if ((nfds_t)i == max_nfds) {
						--max_nfds;
						// shrink max_nfds
						for (int j = i - 1; j > 0; --j) {
							if (fds[j].fd != -1) {
								break;
							}
							else {
								--max_nfds;
							}
						}
					}

				}
				else {  // deal with data
					transform(begin(buf), end(buf), begin(buf),
							[](char c){return toupper(c);});
					send(connfd, (void *)buf, strlen(buf), 0);
				}

				if (--nready == 0) {
					break;
				}
			}
		}
	}

	return 0;
}
