#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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


int main()
{
    int msgid;
    mymsgbuf buf;
	int pid;
    
    msgid = msgget(1234, 0);
    if (msgid < 0)
        err_exit("msgget");

    
    cerr << "IPC MQ was created id: " << msgid << endl;
	
	pid = (int)getpid();
    
    while (1)
    {
        memset(&buf, 0, sizeof(buf));
        buf.mtype = 1000;
		*((int*)buf.mtext) = pid;
        cout << "please input data:";
        cin >> (buf.mtext + sizeof(int*));
        cerr << "buf.mtext:" << buf.mtext << endl;
		cerr << "buf.mtext+4:" << buf.mtext + sizeof(int*) << endl;
		cerr << "*((int*)buf.mtext):" << *((int*)buf.mtext) << endl;
		cerr << "buf.mtext strlen:" << strlen(buf.mtext + sizeof(int*)) << endl;
		
		// send message
		
        if ((msgsnd(msgid, (void*)&buf, sizeof(int*) + strlen(buf.mtext + sizeof(int*)), 0) < 0))
        {
            if (errno == EINTR)
                continue;
            err_exit("msgsnd");
        }
        
        //cout << "buf.mtext:" << buf.mtext << endl;
        
        if (strncmp(buf.mtext + sizeof(int*), "exit", 4) == 0)
        {
            exit(0);
        }
		
		//ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,
        //            int msgflg);
		int msgRcvId;
		sleep(1);
		msgRcvId = msgget(pid, 0);
		if (msgRcvId < 0)
			err_exit("msgRcvId msgget");
		
		if (msgrcv(msgRcvId, (void*)&buf, sizeof(buf.mtext), pid, 0) < 0)
			err_exit("msgrcv");
		cout << "receive:" << buf.mtext << endl;
    }
    
    cout << "program end" << endl;
	
    return 0;
}
