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
    sockaddr_in connaddr;
    socklen_t addrlen;
    
    char buf[1024] = { 0 };
    
    char buf_clientaddr[16] = { 0 };
    char buf_send[] = "bye !";
    
    // 1. socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 2. bind
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);
    
    bind(sockfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    
    // 3.deal with data
    bzero(&connaddr, sizeof(connaddr));
    addrlen = sizeof(connaddr);
    
    recvfrom(sockfd, buf, sizeof(buf), 0, (sockaddr *)&connaddr, &addrlen);
    inet_ntop(AF_INET, (void *)&connaddr.sin_addr.s_addr, buf_clientaddr, sizeof(connaddr));
    cout << "client ip: " << buf_clientaddr
            << ". client port: " << connaddr.sin_port
            << " connected. message is: " << buf << endl;
    sendto(sockfd, (void *)buf_send, strlen(buf_send), 0, (sockaddr *)&connaddr, sizeof(connaddr));
    
    close(sockfd);
    
    
    return 0;
}

