#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <unistd.h>

using namespace std;
using namespace sf;
// Define game board size
#define ROWS 30
#define COLS 30
#define CELL_SIZE 32 // Size of each cell in pixels


int pacman_x = CELL_SIZE + 25/8;
int pacman_y = CELL_SIZE + 25/4;

int gameMapSkeleton[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
int gameMap[ROWS][COLS] = {0};
int totalScorePallet;
int Score;
// shared memory of user direcion
struct shared
{
    int pacman_direction = 0;
}sharedmem;
int closwindow = 0;

// Ghst structure
struct GhostData {
    int x;
    int y;
    int direction=2;
    // Add any other relevant data here
};

// Mutex to protect user input
pthread_mutex_t SharedmemMutex = PTHREAD_MUTEX_INITIALIZER;

// Mutex on gameBoard
pthread_mutex_t GameBoardMutex = PTHREAD_MUTEX_INITIALIZER;

// function that puts score pallets in gamegrid
void intitializeGrid()
{
    totalScorePallet = 0;
    Score = 0;
    // clear game map
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            gameMap[i][j] = 0;
        }
    }
    // in the gameMapSkeleton wherever there is a 0 put a score pallet there
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            if(gameMapSkeleton[i][j] == 0)
            {
                gameMap[i][j] = 2;
                totalScorePallet++;
            }
            else
            gameMap[i][j] = gameMapSkeleton[i][j];
        }
    }
}

// Function to draw the grid with appropriate shapes for pellets, power-ups, and walls
void drawGrid(sf::RenderWindow& window)
{
    sf::CircleShape pelletShape(5);
    sf::CircleShape powerUpShape(5);
    sf::RectangleShape wallShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)); // wall thickness = 1
    sf::Color darkBlue(0, 0, 139);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // Draw pellets, power-ups, and walls
            // lock shared gameBoard mutrx
            pthread_mutex_lock(&GameBoardMutex);
            switch (gameMap[i][j])
            {
                case 2:
                    // Draw small white circle for pellet
                    pelletShape.setFillColor(sf::Color::White);
                    pelletShape.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - 5, i * CELL_SIZE + CELL_SIZE / 2 - 5);
                    window.draw(pelletShape);
                    break;
                
                case 1:
                    // Draw blue rectangle for wall
                   
                    // Set fill color of wallShape to dark blue
                    wallShape.setFillColor(darkBlue);
                    wallShape.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                    window.draw(wallShape);
                    break;
                case -1:
                    // Draw yellow rectangle for ghost house
                    wallShape.setFillColor(sf::Color::Magenta);
                    wallShape.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                    window.draw(wallShape);
                    break;
               
            }
            // unlock mutex
            pthread_mutex_unlock(&GameBoardMutex);


        }
    }
}


//User
void* ghostController(void* arg) {
    // Unpack arguments
    GhostData* ghost = (GhostData*)arg;

    // Ghost controller logic
    while (true) 
    {
        int directionX = 0;
        int directionY = 0;
        
        bool chosen = false;
        pthread_mutex_lock(&GameBoardMutex);
        if (ghost->direction == 1 || ghost->direction == 2) { // Up or down
            if (rand() % 2 == 0) { // Randomly choose left or right
                if (gameMap[ghost->y / CELL_SIZE][(ghost->x + CELL_SIZE) / CELL_SIZE] != 1 &&
                    gameMap[ghost->y / CELL_SIZE][(ghost->x + CELL_SIZE) / CELL_SIZE] != -1 &&
                    gameMap[ghost->y / CELL_SIZE][(ghost->x + CELL_SIZE) / CELL_SIZE] != -2) {
                    directionX = 1;
                    chosen = true;
                    ghost->direction = 4;
                }
            } else {
                if (gameMap[ghost->y / CELL_SIZE][(ghost->x - CELL_SIZE) / CELL_SIZE] != 1 &&
                    gameMap[ghost->y / CELL_SIZE][(ghost->x - CELL_SIZE) / CELL_SIZE] != -1 &&
                    gameMap[ghost->y / CELL_SIZE][(ghost->x - CELL_SIZE) / CELL_SIZE] != -2) {
                    directionX = -1;
                    chosen = true;
                    ghost->direction = 3;
                }
            }
        } else if (ghost->direction == 3 || ghost->direction == 4) { // Left or right
            if (rand() % 2 == 0) { // Randomly choose up or down
                if (gameMap[(ghost->y + CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != 1 &&
                    gameMap[(ghost->y + CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -1 &&
                    gameMap[(ghost->y + CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -2) {
                    directionY = 1;
                    chosen = true;
                    ghost->direction = 1;
                }
            } else {
                if (gameMap[(ghost->y - CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != 1 &&
                    gameMap[(ghost->y - CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -1 &&
                    gameMap[(ghost->y - CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -2) {
                    directionY = -1;
                    chosen = true;
                    ghost->direction = 2;
                }
            }
        }
        pthread_mutex_unlock(&GameBoardMutex);
        if (!chosen) {
            // Continue in current direction
            if (ghost->direction == 1) {
                directionY = 1;
            } else if (ghost->direction == 2) {
                directionY = -1;
            } else if (ghost->direction == 3) {
                directionX = -1;
            } else {
                directionX = 1;
            }
        }

        int nextX = ghost->x + directionX * CELL_SIZE;
        int nextY = ghost->y + directionY * CELL_SIZE;
        
        // Check for collisions with walls
        pthread_mutex_lock(&GameBoardMutex);
        if (nextX >= 0 && nextX < 30 * CELL_SIZE && nextY >= 0 && nextY < 30 * CELL_SIZE &&
            (gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 0 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 2)) {
            ghost->x += directionX * CELL_SIZE;
            ghost->y += directionY * CELL_SIZE;
        }
        pthread_mutex_unlock(&GameBoardMutex);

        //if close
        if(closwindow == 1)
            {
                pthread_exit(NULL);
            }
        // Sleep for a short duration to control the speed of the ghost
        usleep(200000); // Adjust the sleep duration as needed
    }
    pthread_exit(NULL);
}




// Function to handle user input (UI TREAD)
void* userInput(void* arg) {
    // Unpack arguments
    sf::RenderWindow* window = (sf::RenderWindow*) arg;
    while (window->isOpen()) 
    {
        Event event;
        // Process SFML events
        while (window->pollEvent(event)) 
        {
            if (event.type == Event::Closed)
            {   

                closwindow = 1;
                //window->close();
                pthread_exit(NULL);
                //exit(1);
            }
            else if (event.type == Event::KeyPressed) {
                // Lock mutex before accessing shared variable
                pthread_mutex_lock(&SharedmemMutex);
                if(event.key.code==Keyboard::Up)
                {
                    sharedmem.pacman_direction = 1;
                }
                else if(event.key.code==Keyboard::Down)
                {
                    sharedmem.pacman_direction = 2;
                }
                else if(event.key.code==Keyboard::Left)
                {
                    sharedmem.pacman_direction = 3;
                }
                else if(event.key.code==Keyboard::Right)
                {
                    sharedmem.pacman_direction = 4;
                }
                // Unlock mutex after updating shared variable
                pthread_mutex_unlock(&SharedmemMutex);
            }
        }
    }
    pthread_exit(NULL);
}
void movePacman()
{
    int pacman_direction_x = 0;
    int pacman_direction_y = 0;
    int pacdirection;
    pthread_mutex_lock(&SharedmemMutex);
    pacdirection = sharedmem.pacman_direction;
    pthread_mutex_unlock(&SharedmemMutex);
    // check direction of pacman and move it accordingly
    switch (pacdirection) {
        case 1: // Up
            pacman_direction_x = 0;
            pacman_direction_y = -1;
            break;
        case 2: // Down
            pacman_direction_x = 0;
            pacman_direction_y = 1;
            break;
        case 3: // Left
            pacman_direction_x = -1;
            pacman_direction_y = 0;
            break;
        case 4: // Right
            pacman_direction_x = 1;
            pacman_direction_y = 0;
            break;
        default:
            break;
    }
    // boundary detection
    int nextX = pacman_x + pacman_direction_x * CELL_SIZE;
    int nextY = pacman_y + pacman_direction_y * CELL_SIZE;
    pthread_mutex_lock(&GameBoardMutex);
    if (abs(gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE]) == 1 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == -1 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == -2) {
           // cout << "Wall detected!" << endl;
            // set to the old direction

        } 
    else{
    // Update pacman's position
    pacman_x += pacman_direction_x * CELL_SIZE;
    pacman_y += pacman_direction_y * CELL_SIZE;
    }

    // ScorePallet Detection
    if (abs(gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE]) == 2) 
    {
        // ScorePallet found, update gameMap and increment score
        gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] = 0;
        totalScorePallet--;
        Score++;
        //cout<<Score<<endl;
    } 
    pthread_mutex_unlock(&GameBoardMutex);
}

int main() {
    //XInitThreads();
    // Initialize random seed
    srand(time(nullptr));
    // Initialize game board
    intitializeGrid();

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML window");
    // Create the yellow circle (player)
    sf::CircleShape pacman_shape(25/2);
    sf::CircleShape ghost_shape1(25/2);
    sf::CircleShape ghost_shape2(25/2);

    pacman_shape.setFillColor(sf::Color::Yellow);
    ghost_shape1.setFillColor(sf::Color::Red);
    ghost_shape2.setFillColor(sf::Color::Blue);
    pacman_shape.setPosition(25/8, 25/4); // Set initial position to (100, 50)
    // set ghost position to (10, 10)
    ghost_shape1.setPosition(35, 38);
    ghost_shape2.setPosition(65, 38);
    // Create thread for user input
    pthread_t userInputThread;
    pthread_create(&userInputThread, nullptr, userInput, &window);

    // Initialize ghosts' data
    GhostData ghost1 = {35, 38}; // Example initial position
    GhostData ghost2 = {899, 38}; // Example initial position
    ghost2.direction = 3;

    // Initialize other ghost data as needed

    // Create threads for ghost controllers
    pthread_t ghost1Thread;
    pthread_t ghost2Thread;

    pthread_create(&ghost1Thread, nullptr, ghostController, &ghost1);
    pthread_create(&ghost2Thread, nullptr, ghostController, &ghost2);
    // Main loop
    while (window.isOpen()) 
    {
        // Clear, draw, and display
        movePacman();
        window.clear();
        drawGrid(window);
        //moveGhost(ghost1);
        pacman_shape.setPosition(pacman_x, pacman_y); // Update pacman position
        ghost_shape1.setPosition(ghost1.x, ghost1.y); // Update ghost position
        ghost_shape2.setPosition(ghost2.x, ghost2.y); // Update ghost position
        window.draw(pacman_shape); // Draw the player (yellow circle)
        window.draw(ghost_shape1); // Draw the ghost (red circle)
        window.draw(ghost_shape2);
        window.display();
        usleep(150000); // Sleep for 0.3 seconds
        if(closwindow == 1)
        {
            window.close();
        }
        cout<<"pacman_x: "<<pacman_x<<" pacman_y: "<<pacman_y<<endl;
    }

    // Join threads
    pthread_join(userInputThread, nullptr);
    pthread_join(ghost1Thread, nullptr);
    //pthread_join(moveThread, nullptr);
    // Destroy mutexes
    pthread_mutex_destroy(&SharedmemMutex);
    //pthread_mutex_destroy(&pacmanMutex);

    return 0;
}



        
