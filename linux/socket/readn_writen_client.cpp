#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <string.h>

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

int main()
{
	int cli_fd;
	sockaddr_in srv_addr;
	packet send_packet;
	packet recv_packet;

	cli_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (cli_fd < 0)
	{
		cerr << "socket error" << endl;
		exit(1);
	}

	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(8888);
	srv_addr.sin_addr.s_addr = inet_addr("192.168.128.130");

	if (connect(cli_fd, (sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
	{
		perror("connect error");
		exit(1);
	}

	// read file
	string str;
	string strall;

	ifstream ifs("readme.txt");
	if (!ifs)
	{
		cerr << "ifstream error" << endl;
		exit(1);
	}

	while (getline(ifs, str))
	{
		strall += str;
	}

	memset(&send_packet, 0, sizeof(send_packet));
	memset(&recv_packet, 0, sizeof(recv_packet));

	strcpy(send_packet.buf, "strall.c_str()");
	int n = strlen(send_packet.buf);
	send_packet.len = htonl(n);

	cout << "send data:"
		<< send_packet.buf << endl;

	int ret;
	writen(cli_fd, (const void*)&send_packet, 4 + n);

	cerr << "writen finished" << endl;
	n = -1;

	ret = readn(cli_fd, &recv_packet.len, 4);
	cerr << "recv_packet.len: " << recv_packet.len << endl;
	if (ret < 0)
	{
		perror("readn error 4 bytes -1");
		exit(1);
	}
	else if (ret < 4)
	{
		cerr << "server closed" << endl;
		exit(1);
	}

	n = ntohl(recv_packet.len);
	cerr << "n: " << n << endl;
	ret = readn(cli_fd, recv_packet.buf, n);
	if (ret < 0)
	{
		cerr << "readn error" << endl;
		perror("readn error data -1");
		exit(1);
	}
	else if (ret < n)
	{
		cerr << "readn error" << endl;
		exit(1);
	}
	cout << recv_packet.buf << endl;

	close(cli_fd);

	return 0;
}
