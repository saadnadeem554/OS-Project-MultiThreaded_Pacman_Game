#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <unistd.h>
#include <queue>
#include <stack>
#include <vector>
#include <climits>
#include <semaphore.h>
#include <list>
#include <iostream>
#include <string>
#include <mutex>
using namespace std;
using namespace sf;

// Ghst structure
struct GhostData
{
    int x;
    int y;
    int ghostID;
    int direction = 2;
    int speed = 0;
    // add a clock to keep track of time of speed
    sf::Clock speedClock;
    // Add any other relevant data here
    bool hasKey=0;
    bool hasPermit=0;
    bool isActivated=0;
    int pr = 0;
};
sem_t speedBoostSemaphore;
std::list<GhostData*> ghostlist; // Declare list of pointers to GhostData
// add a mutex to protect the ghost list
pthread_mutex_t ghostlistMutex = PTHREAD_MUTEX_INITIALIZER;
void grantspeedboost()
{
    // Try to acquire the speed boost semaphore
    if (sem_trywait(&speedBoostSemaphore) == 0) 
    {
        // Get a pointer to the first ghost in the list
        pthread_mutex_lock(&ghostlistMutex);
        GhostData* ghost = ghostlist.front();
        // Set the speed of the ghost to 1
        ghost->speed = 1;
        ghost->speedClock.restart();
        // Print the message
        cout << "Speed boost granted to ghost " << ghost->ghostID << endl;
        // Release the mutex
        pthread_mutex_unlock(&ghostlistMutex);
    }
}

void updatespeedboost()
{
    // Check if ghost list is not empty
    if (!ghostlist.empty())
    {
        pthread_mutex_lock(&ghostlistMutex);
        // Get a pointer to the first ghost in the list
        GhostData* ghost = ghostlist.front();
        // Check if speed boost is active and time limit exceeded
        if (ghost->speed && ghost->speedClock.getElapsedTime().asSeconds() > 3)
        {
            // Remove the speed boost
            ghost->speed = 0;
            // Print the message
            cout << "Speed boost removed from ghost " << ghost->ghostID << endl;
            // Pop the first ghost from the list
            ghostlist.pop_front();
            // If ghost priority is high, push it to the back
            if (ghost->pr > 1)
            {
                ghostlist.push_back(ghost);
            }
            cout << "List of ghosts: ";
        for (auto it = ghostlist.begin(); it != ghostlist.end(); ++it)
        {
            cout << (*it)->ghostID << " ";
        }
        cout << endl;
            // Release the semaphore
            sem_post(&speedBoostSemaphore);
        }
        pthread_mutex_unlock(&ghostlistMutex);
    }
}


void *ghost(void *arg)
{
    GhostData *ghost = (GhostData *)arg;
    while (1)
    {
        // Output the speed of the ghost
        //cout << "Speed of ghost " << ghost->ghostID << " is " << ghost->speed << endl;

        // Check if the speed boost is granted
        if (ghost->pr == 1 && ghost->speed == 0 && ghost->speedClock.getElapsedTime().asSeconds() > 7)
        {
            ghost->speedClock.restart();
            // Push the ghost back to the end of ghostlist
            pthread_mutex_lock(&ghostlistMutex);
            GhostData* temp = ghostlist.front();
            ghostlist.pop_front();
            ghostlist.push_front(ghost);
            ghostlist.push_front(temp);
            pthread_mutex_unlock(&ghostlistMutex);
            updatespeedboost();
            // Grant the speed boost
            grantspeedboost();
        }
        else if (ghost->pr != 1 && ghost->speed == 0)
        {
            // Grant the speed boost
            grantspeedboost();
        }

        // Update the speed boost
        updatespeedboost();

        // print the list of ghosts
        
        //cout<<"THREAD ECEC" <<ghost->ghostID<<endl;
        // Sleep for 1 second
        //sleep(1);
    }
}

int main()
{
    // Initialize semaphore
    sem_init(&speedBoostSemaphore, 0, 2);
    // create 4 ghosts
    GhostData ghost1;
    ghost1.ghostID = 1;
    ghost1.pr = 1;
    
    GhostData ghost2;
    ghost2.ghostID = 2;
    ghost2.pr = 2;
    ghostlist.push_back(&ghost2);
   
    GhostData ghost3;
    ghost3.ghostID = 3;
    ghost3.pr = 1;
    //ghostlist.push_back(&ghost3);
    GhostData ghost4;   
    ghost4.ghostID = 4;
    ghost4.pr = 3;
    ghostlist.push_back(&ghost4);
    
    pthread_t ghost1Thread;
    pthread_t ghost2Thread;
    pthread_t ghost3Thread;
    pthread_t ghost4Thread;

    // restart clocks 
    ghost1.speedClock.restart();
    ghost2.speedClock.restart();
    ghost3.speedClock.restart();
    ghost4.speedClock.restart();


    pthread_create(&ghost1Thread, nullptr, ghost, &ghost1);
    pthread_create(&ghost2Thread, nullptr, ghost, &ghost2);
    pthread_create(&ghost3Thread, nullptr, ghost, &ghost3);
    pthread_create(&ghost4Thread, nullptr, ghost, &ghost4);
    // join the ghost thread
    pthread_join(ghost1Thread, NULL);
    pthread_join(ghost2Thread, NULL);
    pthread_join(ghost3Thread, NULL);
    pthread_join(ghost4Thread, NULL);
    // destroy the semaphore
    sem_destroy(&speedBoostSemaphore);
    return 0;
}