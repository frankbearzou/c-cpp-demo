#include <cstdlib>
#include <iostream>
#include <stack>
#include <map>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

stack<int> sta;
sem_t sem_blank, sem_product;
pthread_mutex_t mutex; // = PTHREAD_MUTEX_INITIALIZER;
map<int, int> map_pro, map_con;

void* producer(void *param)
{
    int value = 0;
    for (int i = 0; i < 100; ++i)
    {
        if (sem_wait(&sem_blank) != 0)
        {
            perror("sem_wait");
            exit(1);
        }
        if (pthread_mutex_lock(&mutex) != 0)
        {
            perror("pthread_mutex_lock");
            exit(2);
        }
        if (sta.size() == 0)
        {
            value = 0;
        }
        else
        {
            value = sta.top();
        }
        ++value;
        sta.push(value);
        ++map_pro[value];
        cout << "producer: " << pthread_self()
                << ", size:" << sta.size()
                << ", value:" << sta.top() << endl;
        if (pthread_mutex_unlock(&mutex) != 0)
        {
            perror("pthread_mutex_unlock");
            exit(3);
        }
        if (sem_post(&sem_product) != 0)
        {
            perror("sem_pose");
            exit(4);
        }
        usleep(2);
    }
}

void* consumer(void *param)
{
    int value = 0;
    for (int i = 0; i < 100; ++i)
    {
        if (sem_wait(&sem_product) != 0)
        {
            perror("sem_wait");
            exit(6);
        }
        if (pthread_mutex_lock(&mutex) != 0)
        {
            perror("pthread_mutex_lock");
            exit(5);
        }
        
        value = sta.top();
        sta.pop();
        ++map_con[value];
        cout << "consumer: " << pthread_self()
                << ", size: " << sta.size()
                << ", value: " << value << endl;
        
        if (pthread_mutex_unlock(&mutex) != 0)
        {
            perror("pthread_mutex_unlock");
            exit(8);
        }
        if (sem_post(&sem_blank) != 0)
        {
            perror("sem_post");
            exit(7);
        }
        
        usleep(2);
    }

}

int main(int argc, char** argv)
{
    int const NUM_PRODUCER_PTHREAD = 10;
    int const NUM_CONSUMER_PTHREAD = 10;
    pthread_t pid_pro[NUM_PRODUCER_PTHREAD], pid_con[NUM_CONSUMER_PTHREAD];
    
    sem_init(&sem_blank, 0, 50);
    sem_init(&sem_product, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    // cout.sync_with_stdio(false);
    
    for (int i = 0; i < 10; ++i)
    {
        pthread_create(&pid_pro[i], NULL, producer, NULL);
    }
    
    for (int i = 0; i < 10; ++i)
    {
        pthread_create(&pid_con[i], NULL, consumer, NULL);
    }
    
    for (int i = 0; i < 10; ++i)
    {
        if (pthread_join(pid_pro[i], NULL) != 0)
            perror("pthread_join");
    }
    for (int i = 0; i < 10; ++i)
    {
        if (pthread_join(pid_con[i], NULL) != 0)
            perror("pthread_join");
    }
    
    cout << "finished" << endl;
    
    if (pthread_mutex_destroy(&mutex) != 0)
    {
        perror("pthread_mutex_destroy");
    }
    
    if (sem_destroy(&sem_blank) != 0)
    {
        perror("sem_destroy");
    }
    sem_destroy(&sem_product);
    
    cout << "in main, stack size: " << sta.size() << endl;
    while (sta.size() != 0)
    {
        cout << sta.top() << " ";
    }
    cout << endl;
    if (map_pro == map_con)
    {
        cout << "map equal" << endl;
    }
    else
    {
        cout << "map not equal" << endl;
    }
    

    for (const auto &p : map_pro)
    {
        cout << "[" << p.first << "," << p.second << "] ";
    }
    cout << endl;
    for (const auto &p : map_con)
    {
        cout << "[" << p.first << "," << p.second << "] ";
    }
    cout << endl;
    
    int sum = 0;
    for (const auto &p : map_pro)
    {
        sum += p.second;
    }
    cout << "sum map_pro = " << sum << endl;
    
    sum = 0;
    for (const auto &p : map_con)
    {
        sum += p.second;
    }
    cout << "sum map_con = " << sum << endl;
    
    for (int i = 0;i <= 50; ++i)
    {
        if (map_pro[i] != map_con[i])
        {
            cout << "i: " << i << ", [" << map_pro[i] << ", " << map_con[i] << "]." << endl;
        }
    }

    return 0;
}
