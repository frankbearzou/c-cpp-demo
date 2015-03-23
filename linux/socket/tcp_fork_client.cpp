#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv)
{
    const char *SERVER_IP = "192.168.128.130";
    uint16_t SERVER_PORT = 8000;
    int serverfd;
    sockaddr_in serveraddr;
    char buf[1024] = { 0 };
    
    // 1. socket
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. connect
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, (void *)&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    
    connect(serverfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    
    while (1)
    {
        cin >> buf;
        if (strcmp(buf, "quit") == 0)
        {
            close(serverfd);
        }
        send(serverfd, (void *)buf, strlen(buf), 0);
        bzero(buf, sizeof(buf));
        recv(serverfd, buf, sizeof(buf), 0);
        cout << buf << endl;
    }
    
    

    return 0;
}

