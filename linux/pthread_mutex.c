#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>

/*
 * 
 */
int volatile counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* fn(void* param)
{
    int val = 0;
    int i = 0;
    for (i = 0; i < 5000; ++i)
    {
        pthread_mutex_lock(&mutex);
        ++counter;
        printf("thread %ld, counter = %d. \n", pthread_self(), counter);
        pthread_mutex_unlock(&mutex);
        usleep(1);
    }
}

int main(int argc, char** argv, char** environ)
{
    pthread_t pid1, pid2;
    
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&pid1, NULL, fn, NULL);
    pthread_create(&pid2, NULL, fn, NULL);
    
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    puts("after pthread_join.....");
    
    //sleep(2);
    pthread_mutex_destroy(&mutex);
    
    printf("in main counter = %d\n", counter);
    
    return (EXIT_SUCCESS);
}
