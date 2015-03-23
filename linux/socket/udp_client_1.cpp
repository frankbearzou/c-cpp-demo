#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

const char *SERVER_IP = "192.168.128.130";
uint16_t SERVER_PORT = 8000;

int main(int argc, char** argv)
{
    int sockfd;
    sockaddr_in serveraddr;
    socklen_t serveraddrlen;
    int recvlen;
    
    char buf[1024] = { 0 };
    
    // 1. socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 2. deal with data
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, (void *)&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    
    strcat(buf, "hello, server");
    sendto(sockfd, (void *)buf, strlen(buf), 0, (sockaddr *)&serveraddr, sizeof(serveraddr));
    
    bzero(&buf, sizeof(buf));
    serveraddrlen = sizeof(serveraddr);
    recvlen = recvfrom(sockfd, (void *)buf, sizeof(buf), 0, (sockaddr *)&serveraddr, &serveraddrlen);
    cout << "received message: " << buf << endl;
    
    close(sockfd);

    return 0;
}

