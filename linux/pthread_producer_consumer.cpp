#include <cstdlib>
#include <iostream>
#include <stack>
#include <pthread.h>
#include <unistd.h>

using namespace std;

stack<int> sta;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void *param)
{
    int value = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (sta.size() != 0)
        {
            value = sta.top();
        }
        else
        {
            value = 0;
        }
        ++value;
        sta.push(value);
        cout << "producer size: " << sta.size() << ", value:" << sta.top() << endl;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        usleep(20);
    }

}

void* consumer(void *param)
{
    int value = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (sta.size() == 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        value = sta.top();
        sta.pop();
        cout << "consumer size: " << sta.size() << ", value: " << value << endl;
        pthread_mutex_unlock(&mutex);
        usleep(20);
    }

}

int main(int argc, char** argv)
{
    pthread_t pid_pro, pid_con;
    pthread_create(&pid_pro, NULL, producer, NULL);
    pthread_create(&pid_con, NULL, consumer, NULL);

    pthread_join(pid_pro, NULL);
    pthread_join(pid_con, NULL);


    cout << "hello" << endl;
    return 0;
}
