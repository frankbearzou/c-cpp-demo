#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>


using namespace std;

const unsigned int SERVER_PORT = 8000;

int main(int argc, char** argv)
{
    int sockfd;
    sockaddr_in servaddr;
    int yes = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *) &yes, sizeof (yes));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void *) &yes, sizeof (yes));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (sockaddr *) & servaddr, sizeof (servaddr));

    listen(sockfd, 128);

    while (1)
    {
        int connfd;
        sockaddr_in connaddr;
        int connport;
        char connaddr_str[128] = {0};
        char str_ip[128] = {0};
        char str_port[128] = {0};
        char str_welcome[128] = "welcome ";

        char recv_buf[1024] = {0};
        int recv_num = 0;


        bzero(&connaddr, sizeof (connaddr));
        connfd = accept(sockfd, (sockaddr *) & connaddr, (unsigned int*) &connport);

        strcat(str_ip, inet_ntop(AF_INET, (void *) &connaddr.sin_addr.s_addr,
                connaddr_str, (unsigned int) sizeof (connaddr_str))
                );
        sprintf(str_port, "%u", ntohs(connaddr.sin_port));

        cout << "ip: " << str_ip << ", port: " << str_port << " connected !" << endl;

        sprintf(str_welcome, "welcome ip: %s, port:%s here.\n", str_ip, str_port);

        send(connfd, str_welcome, sizeof (str_welcome), 0);

        recv_num = recv(connfd, (void *) recv_buf, sizeof (recv_buf), 0);

        strcat(recv_buf, " Bye !");
        send(connfd, (void *) recv_buf, sizeof (recv_buf), 0);
        close(connfd);
        bzero(recv_buf, sizeof (recv_buf));
    }

    close(sockfd);
    shutdown(sockfd, SHUT_RD);

    return 0;
}

