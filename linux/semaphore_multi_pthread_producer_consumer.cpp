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
pthread_mutex_t mutex_producer; // = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_consumer; // = PTHREAD_MUTEX_INITIALIZER;
map<int, int> map_pro, map_con;

void* producer(void *param)
{
    int value = 0;
    for (int i = 0; i < 100; ++i)
    {
        sem_wait(&sem_blank);
        pthread_mutex_lock(&mutex_producer);
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
        pthread_mutex_unlock(&mutex_producer);
        sem_post(&sem_product);
        usleep(2);
    }
}

void* consumer(void *param)
{
    int value = 0;
    for (int i = 0; i < 100; ++i)
    {
        
        sem_wait(&sem_product);
        pthread_mutex_lock(&mutex_consumer);
        value = sta.top();
        sta.pop();
        ++map_con[value];
        cout << "consumer: " << pthread_self()
                << ", size: " << sta.size()
                << ", value: " << value << endl;
        pthread_mutex_unlock(&mutex_consumer);
        sem_post(&sem_blank);
        
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
    pthread_mutex_init(&mutex_producer, NULL);
    pthread_mutex_init(&mutex_consumer, NULL);
    
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
        pthread_join(pid_pro[i], NULL);
    }
    for (int i = 0; i < 10; ++i)
    {
        pthread_join(pid_con[i], NULL);
    }
    
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

    return 0;
}
