#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <cstdarg>

using namespace std;

static void anetSetError(char *err, const char *fmt, ...)
{
    va_list ap;

    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, 256, fmt, ap);
    va_end(ap);
}

int anetKeepAlive(char *err, int fd, int interval)
{
    int val = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == -1)
    {
        anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
        return -1;
    }

#ifdef __linux__
    /* Default settings are more or less garbage, with the keepalive time
     * set to 7200 by default on Linux. Modify settings to make the feature
     * actually useful. */

    /* Send first probe after interval. */
    val = interval;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
        anetSetError(err, "setsockopt TCP_KEEPIDLE: %s\n", strerror(errno));
        return -1;
    }

    /* Send next probes after the specified interval. Note that we set the
     * delay as interval / 3, as we send three probes before detecting
     * an error (see the next setsockopt call). */
    val = interval/3;
    if (val == 0) val = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
        anetSetError(err, "setsockopt TCP_KEEPINTVL: %s\n", strerror(errno));
        return -1;
    }

    /* Consider the socket in error state after three we send three ACK
     * probes without getting a reply. */
    val = 3;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
        anetSetError(err, "setsockopt TCP_KEEPCNT: %s\n", strerror(errno));
        return -1;
    }
#endif

    return 0;
}

void sig_child(int num)
{
    waitpid(0, NULL, WNOHANG);
}

int main(int argc, char** argv)
{
    const char *SERVER_IP = "192.168.128.130";
    uint16_t SERVER_PORT = 8000;
    int sockfd;
    sockaddr_in serveraddr;
    int keepalive = 1;
    
    signal(SIGCHLD, sig_child);
    
    // 1. socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &keepalive, sizeof(keepalive));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &keepalive, sizeof(keepalive));
    
    if (anetKeepAlive("keepalive", sockfd, 300) == -1)
        cerr << "anetKeepAlive" << endl;
    
    
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
                if (recvlen == 0) {
                    cerr << "the other side is closed" << endl;
                    break;
                }
                write(STDOUT_FILENO, buf, recvlen);
                cout << endl;
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

