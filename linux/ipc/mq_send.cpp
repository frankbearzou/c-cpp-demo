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
    char mtext[100];    /* message data */
};


int main()
{
    int msid;
    mymsgbuf buf;
    
    msid = msgget(1234, IPC_CREAT | 0666);
    if (msid < 0)
    {
        perror("msgget");
        exit(-1);
    }
    
    cout << "IPC MQ was created id: " << msid << endl;
    
    while (1)
    {
        memset(&buf, 0, sizeof(buf));
        buf.mtype = 1001;
        cout << "please input data:";
        //fgets(buf.mtext, sizeof(buf.mtext), stdin);
        cin >> buf.mtext;
        
        if ((msgsnd(msid, (void*)&buf, strlen(buf.mtext), 0) < 0))
        {
            if (errno == EINTR)
                continue;
            perror("msgsnd");
            exit(-1);
        }
        
        cout << "buf.mtext:" << buf.mtext << endl;
        
        if (strncmp(buf.mtext, "exit", 4) == 0)
        {
            break;
        }
    }
    
    cout << "program end" << endl;
	
    return 0;
}
