#include <cstdlib>
#include <iostream>
#include <stack>
#include <map>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

using namespace std;

struct MM
{
    int num = 0;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};

int main(int argc, char** argv)
{
    MM *pmm;
    int fd;
    pid_t pid;
    int ret = 0;

    fd = open("map", O_CREAT|O_RDWR|O_EXCL, 0777);
    unlink("map");
    
    ftruncate(fd, sizeof(*pmm));
    pmm = (MM*)mmap(NULL, sizeof(*pmm), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (pmm == MAP_FAILED)
    {
        perror("mmap");
    }
    
    memset(pmm, 0, sizeof(*pmm));

    cout << "after close" << endl;
    if ((ret = pthread_mutexattr_init(&pmm->mutexattr)) != 0)
    {
        cout << ret << endl;
        perror("pthread_mutexattr_init");
    }

    if (pthread_mutexattr_setpshared(&pmm->mutexattr, PTHREAD_PROCESS_SHARED) != 0)
    {
        perror("pthread_mutexattr_setpshared");
    }

    if (pthread_mutex_init(&pmm->mutex, &pmm->mutexattr) != 0)
    {
        perror("pthread_mutex_init");
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        _exit(-1);
    }
    else if (pid == 0)
    { // child
        for (int i = 0; i < 500; ++i)
        {
            pthread_mutex_lock(&pmm->mutex);
            ++pmm->num;
            pthread_mutex_unlock(&pmm->mutex);
            usleep(2);
        }
        _exit(0);
    }
    else
    { // parent
        for (int i = 0; i < 500; ++i)
        {
            pthread_mutex_lock(&pmm->mutex);
            pmm->num += 2;
            pthread_mutex_unlock(&pmm->mutex);
            usleep(2);
        }
        
    }
    
    cout << "wait child" << endl;
    if (waitpid(pid, NULL, 0) == -1)
    {
        perror("waitpid");
        exit(1);
    }
    
    cout << "num = " << pmm->num << endl;
    pthread_mutexattr_destroy(&pmm->mutexattr);
    pthread_mutex_destroy(&pmm->mutex);
    if (munmap(pmm, sizeof(MM)) != 0)
    {
        perror("munmap");
        exit(2);
    }
    close(fd);

    return 0;
}
