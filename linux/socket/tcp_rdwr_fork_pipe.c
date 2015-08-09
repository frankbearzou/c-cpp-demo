#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096

void handler(int signum, siginfo_t *sig, void *p)
{
    int connfd;
    // main process
    if (signum == SIGRTMIN + 1)
    {
        connfd = sig->si_value.sival_int;
        printf("pid: %d in handler, connfd: %d\n", getpid(), connfd);
        
        close(connfd);
    }
}

int main()
{
    int sockfd;
    int optval = 1;
    struct sockaddr_in sock_addr;
    struct sigaction act;

    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGRTMIN + 1, &act, NULL) < 0)
    {
        perror("sigaction error");
        exit(1);
    }

    if (signal(SIGCHLD, SIG_IGN) < 0)
    {
        perror("signal error");
        exit(1);
    }
    
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
    {
    	perror("socket error");
    	exit(-1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval)) < 0)
    {
        perror("setsockopt error");
        exit(1);
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(8888);

    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr) ) < 0)
    {
    	perror("bind error");
    	exit(-2);
    }

    if (listen(sockfd, SOMAXCONN) < 0)
    {
    	perror("listen error");
    	exit(-3);
    }

    while (1)
    {
    	int connfd;
    	struct sockaddr_in connaddr;
    	socklen_t addr_len;

    	

    	memset((void *)&connaddr, 0, sizeof(connaddr));
    	addr_len = sizeof(connaddr);

    	if ( (connfd = accept(sockfd, (struct sockaddr *)&connaddr, &addr_len) ) < 0 )
    	{
            // signal interrupts accept, ignore signal
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("accept error");
                exit(-4);
            }
    		
    	}

        pid_t pid;

        pid = fork();
        // deal with new connection
        if (pid == 0)
        {
            int pipefd[2];
            pid_t conn_pid;
            char buf[MAXLINE] = { 0 };
            int n;

            if (pipe(pipefd) < 0)
            {
                perror("pipe error");
                exit(1);
            }

            conn_pid = fork();
            // parent receive data from client
            // and send data to child by pipe
            if (conn_pid > 0)
            {
                close(pipefd[0]);
                while ((n = read(connfd, buf, MAXLINE - 1)) > 0)
                {
                    printf("n:%d\n", n);
                    buf[n] = 0;
                    int i;
                    for (i = 0; i < n; ++i)
                    {
                        buf[i] = toupper(buf[i]);
                    }
                    
                    write(pipefd[1], buf, n);
                }

                // clients close connection
                if (n == 0)
                {
                    union sigval value;
                    value.sival_int = connfd;
                    if (sigqueue(getppid(), SIGRTMIN + 1, value) < 0)
                    {
                        perror("sigqueue error");
                        exit(1);
                    }

                    // this is another way to kill child process
                    /*if (sigqueue(conn_pid, SIGRTMIN + 2, value) < 0)
                    {
                        perror("sigqueue error");
                        exit(1);
                    }*/

                    // let child konw parent closed
                    close(pipefd[1]);
                    if (close(connfd) < 0)
                    {
                        perror("close connfd error");
                        exit(1);
                    }
                    exit(0);
                }
            }
            // child send data to client
            else if (conn_pid == 0)
            {
                close(pipefd[1]);
                while ((n = read(pipefd[0], buf, MAXLINE - 1)) > 0)
                {
                    printf("working child n:%d\n", n);
                    buf[n] = 0;
                    printf("%s", buf);
                    write(connfd, buf, n);
                }
                // parent closed
                if (n == 0)
                {
                    printf("working child n=0:%d\n", n);
                    union sigval value;
                    value.sival_int = connfd;

                    close(pipefd[0]);
                    if (close(connfd) < 0)
                    {
                        perror("close connfd error");
                        exit(1);
                    }
                    exit(0);
                }
            }
            else
            {
                perror("fork error");
                exit(1);
            }
            
        }
        else if (pid > 0)
        {
            ;
        }
        else
        {
            perror("fork error");
            exit(1);
        }
    	
    }
    
    return (EXIT_SUCCESS);
}
