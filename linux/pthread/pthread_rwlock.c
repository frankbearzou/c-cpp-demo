/* 
 * File:   main.c
 * Author: coolzeze
 *
 * Created on January 9, 2015, 2:30 PM
 */

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
int counter = 0;
pthread_rwlock_t rwlock;

void* pthr_write(void *param)
{
    int i = 0;
    int value = 0;
    for (i = 0; i < 50000; ++i)
    {
        pthread_rwlock_wrlock(&rwlock);
        value = counter;
        
        ++counter;
        printf("write %ld: old=%d, new=%d\n", pthread_self(), value, counter);
        pthread_rwlock_unlock(&rwlock);
        usleep(2);
    }
}

void* pthr_read(void *param)
{
    int i;
    for (i = 0; i < 50000; ++i)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("read %ld: %d\n", pthread_self(), counter);
        
        pthread_rwlock_unlock(&rwlock);
        usleep(2);
    }
}

int main(int argc, char** argv, char** environ)
{
    int i;
    pthread_t pid[10];
    
    pthread_rwlock_init(&rwlock, NULL);
    
    for (i = 0; i < 3; ++i)
    {
        pthread_create(&pid[i], NULL, pthr_write, NULL);
    }
    for (i = 3; i < 10; ++i)
    {
        pthread_create(&pid[i], NULL, pthr_read, NULL);
    }
    
    for (i = 0; i < 10; ++i)
    {
        pthread_join(pid[i], NULL);
    }

    puts("after pthread_join.....");
    
    //sleep(2);
    pthread_rwlock_destroy(&rwlock);
    
    printf("in main counter = %d\n", counter);
    
    return (EXIT_SUCCESS);
}
