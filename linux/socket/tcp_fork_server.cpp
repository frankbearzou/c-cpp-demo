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
    int sockfd;
    sockaddr_in serveraddr;
    
    // 1. socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. bind
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);
    
    bind(sockfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    
    // 3. listen
    listen(sockfd, 128);
    
    // 4. accept
    while (1)
    {
        pid_t pid;
        int connfd;
        sockaddr connaddr;
        socklen_t connaddrlen;
        
        connaddrlen = sizeof(connaddr);
        connfd = accept(sockfd, (sockaddr *)&connaddr, &connaddrlen);
        
        // fork
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // child 
            close(sockfd);
            char buf[1024] = { 0 };
            int recvlen = 0;
            // deal with data
            while (1)
            {
                recvlen = recv(connfd, (void *)&buf, sizeof(buf), 0);
                write(STDOUT_FILENO, buf, recvlen);
                for (int i = 0; i < recvlen; ++i) {
                    buf[i] = toupper(buf[i]);
                }
                send(connfd, (void *)buf, recvlen, 0);
                bzero(buf, sizeof(buf));
            }
        } else {
            // parent process
            close(connfd);
        }
    }
    
    close(sockfd);
    shutdown(sockfd, 0);
    

    return 0;
}

