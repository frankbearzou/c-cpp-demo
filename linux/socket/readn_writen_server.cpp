#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>

using namespace std;

struct packet
{
	int len;
	char buf[4096];
};

ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char*) buf;
	while (nleft > 0)
	{
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nread == 0)
			return count - nleft;

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char *bufp = (char*) buf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nwritten == 0)
			continue;

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}

void do_service(int conn)
{
	packet rcv_packet;
	int n;

	while (1)
	{
		memset(&rcv_packet, 0, sizeof(rcv_packet));
		int ret = readn(conn, &rcv_packet.len, 4);
		cerr << "rcv_packet.len: " << rcv_packet.len << endl;
		if (ret < 0)
		{
			perror("readn error 4 bytes");
			exit(1);
		}
		else if (ret < 4)
		{	// client closed
			close(conn);
			cerr << "client closed" << endl;
			break;
		}

		// receive data
		n = ntohl(rcv_packet.len);
		cerr << "n: " << n << endl;
		ret = readn(conn, (void *)&rcv_packet.buf, n);
		cerr << "ret: " << ret << endl;
		cerr << "data: " << rcv_packet.buf << endl;
		if (ret < 0)
		{
			perror("readn error data");
			exit(1);
		}
		else if (ret < n)
		{
			cerr << "readn read error" << endl;
			break;
		}

		// deal with data
		for_each(begin(rcv_packet.buf), end(rcv_packet.buf), [](char &c){c = toupper(c);});

		// send data
		ret = writen(conn, (const void *)&rcv_packet, n + 4);
		cerr << "writen ret: " << ret << endl;
	}
}

void handler(int sig)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
	{
		;
	}
}

int main()
{
	int srv_fd;
	sockaddr_in srv_addr;
	int sock_opt_yes = 1;
	pid_t pid;

	signal(SIGCHLD, handler);

	srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_fd < 0)
	{
		cerr << "socket error" << endl;
		exit(1);
	}

	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(8888);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	
	if (setsockopt(srv_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt_yes, sizeof(sock_opt_yes)) < 0)
	{
		cerr << "setsockopt error" << endl;
		exit(1);
	}

	if (bind(srv_fd, (sockaddr *)&srv_addr, (socklen_t)sizeof(srv_addr)) < 0)
	{
		cerr << "bind error" << endl;
		exit(1);
	}

	if (listen(srv_fd, SOMAXCONN) < 0)
	{
		cerr << "listen error" << endl;
		exit(1);
	}

	while(1)
	{
		int conn_fd;
		sockaddr_in conn_addr;
		socklen_t conn_len;

		conn_len = sizeof(conn_addr);
		memset(&conn_addr, 0, conn_len);

		conn_fd = accept(srv_fd, (sockaddr *)&conn_addr, &conn_len);
		if (conn_fd < 0)
		{
			if (errno == EAGAIN)
				continue;
			else
			{
				cerr << "accept error" << endl;
				exit(1);
			}
		}

		pid = fork();
		if (pid < 0)
		{
			cerr << "fork error" << endl;
			exit(1);
		}
		else if (pid == 0)
		{
			// child
			close(srv_fd);
			do_service(conn_fd);
			exit(0);
		}
		else
		{
			// parent
			close(conn_fd);
		}
	}


	return 0;
}
