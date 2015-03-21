#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

const int max_number = 5;

struct People
{
    int id;

    int get_righthand_num()
    {
        return id;
    }

    int get_lefthand_num()
    {
        int num = id - 1;
        if (num < 0)
            return max_number - 1;
    }
};

pthread_mutex_t mutex[max_number];
People people[max_number];

void* eating(void *param_id)
{
    int id = *((int*)param_id);
    cout << "id:" << id << endl;

    int lefthand_num = people[id].get_lefthand_num();
    int righthand_num = people[id].get_righthand_num();


    while (1)
    {
        // lock right hand
        pthread_mutex_lock(&mutex[righthand_num]);

        // try to lock left hand
        if (pthread_mutex_trylock(&mutex[lefthand_num]) != 0)
        { // cannot get lock
            // unlock right hand lock
            pthread_mutex_unlock(&mutex[righthand_num]);
        } else
        { // get right hand lock,
            // begin to eat
            cout << people[id].id << " is eating..." << endl;
            sleep(1);
            // unlock two hands
            pthread_mutex_unlock(&mutex[lefthand_num]);
            pthread_mutex_unlock(&mutex[righthand_num]);
        }
        usleep(10);
    }
}

int main(int argc, char** argv)
{
    pthread_t pthr[max_number];
    int param[max_number] = {0, 1, 2, 3, 4};

    for (int i = 0; i < max_number; ++i)
    {
        people[i].id = i;
        pthread_mutex_init(&mutex[i], NULL);
    }

    cout << "1" << endl;
    for (int i = 0; i < max_number; ++i)
    {
        pthread_create(&pthr[i], NULL, eating, (void *)&param[i]);
    }

    for (int i = 0; i < max_number; ++i)
    {
        pthread_join(pthr[i], NULL);
    }

    return 0;
}
