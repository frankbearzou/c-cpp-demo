#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <iostream>

using namespace std;

struct mymsgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[8192];    /* message data */
};

inline void err_exit(const char *msg)
{
	perror(msg);
	exit(-1);
}

void killMsg(int msqid)
{
	//int msgctl(int msqid, int cmd, struct msqid_ds *buf);
	if (msgctl(msqid, IPC_RMID, NULL) < 0)
		err_exit("killMsg msgctl");
}

void sendMsg(char *text)
{
	// child does the job
	int pid = *((int*)text);
	
	// send msg
	int msgid;
	mymsgbuf buf;
	
	if ((msgid = msgget(pid, IPC_CREAT | 0666)) < 0)
		err_exit("sendMsg msgget");
	
	if (strncmp(text + sizeof(int*), "exit", 4) == 0)
	{
		cerr << "program:" << pid << " end" << endl;
		killMsg(msgid);
		exit(0);
	}

	//int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
	memset(&buf, 0, sizeof(buf));
	buf.mtype = pid;
	strncpy(buf.mtext, text + sizeof(int*), strlen(text + sizeof(int*)));
	if (msgsnd(msgid, (void*)&buf, strlen(text + sizeof(int*)), 0) < 0)
		err_exit("sendMsg msgsnd");


	cout << "msg type=" << buf.mtype << ", text=" << buf.mtext << endl;
}

int main(int argc, char* argv[])
{
	int msgid;
	pid_t pid;
	
	if (signal(SIGCHLD, SIG_IGN) < 0)
		err_exit("signal");
	
	if ((msgid = msgget(1234, IPC_CREAT | 0666)) < 0)
		err_exit("msgget");

	
	mymsgbuf buf;
	
	while (1)
	{
		memset(&buf, 0, sizeof(buf));
		if (msgrcv(msgid, (void*)&buf, sizeof(buf.mtext), 1000, 0) < 0)
		{
			if (errno == EINTR)
				continue;
			else if (errno == EINVAL)
				err_exit("Invalid  msqid  value");
		}
		
		pid = fork();
		if (pid == 0)
		{
			//doMsg(msgid);
			sendMsg(buf.mtext);
			_exit(0);
		}
		else if (pid > 0)
		{
			// parent process
		}
		else
			err_exit("fork");
	
		
	}
	
	
	
	return 0;
}
