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
const unsigned int SERVER_PORT = 8000;

int main(int argc, char** argv)
{
    int connfd;
    sockaddr_in serveraddr;
    char buf[1024] = { 0 };
    
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, (void *)&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    
    connect(connfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    
    recv(connfd, buf, sizeof(buf), 0);
    cout << buf << endl;
    send(connfd, (void *)argv[1], sizeof(argv[1]), 0);
    
    bzero(buf, sizeof(buf));
    recv(connfd, buf, sizeof(buf), 0);
    cout << buf << endl;
    
    close(connfd);
    
    return 0;
}

