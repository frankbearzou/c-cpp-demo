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
    char mtext[100];    /* message data */
};

int main(int argc, char* argv[])
{
	int msgid;
	mymsgbuf buf;
	
	
	if (argc == 1)
	{
		cout << "please input mq id you want to read:";
		cin >> msgid;
	}
	else if (argc == 2)
	{
		msgid = atoi(argv[1]);
	}
	
	if (msgget(1234, 0) < 0)
	{
		perror("msgget");
		exit(-1);
	}
	
	while (1)
	{
		memset(&buf, 0, sizeof(buf));
		if (msgrcv(msgid, (void*)&buf, sizeof(buf.mtext), 1001, 0) < 0)
		{
			if (errno == EINTR)
				continue;
			else if (errno == EINVAL)
			{
				perror("Invalid  msqid  value");
				exit(-1);
			}
		}
		
		if (strncmp(buf.mtext, "exit", 4) == 0)
		{
			cerr << "program end" << endl;
			exit(0);
		}
		
		cout << "msg type=" << buf.mtype << ", text=" << buf.mtext << endl;
	}
	
	return 0;
}
