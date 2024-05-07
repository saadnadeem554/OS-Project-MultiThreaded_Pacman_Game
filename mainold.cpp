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

int gameMap[ROWS][COLS] = {
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
         {0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0, 1, 1, 0, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
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
            switch (gameMap[i][j])
            {
                case 2:
                    // Draw small white circle for pellet
                    pelletShape.setFillColor(sf::Color::White);
                    pelletShape.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - 5, i * CELL_SIZE + CELL_SIZE / 2 - 5);
                    window.draw(pelletShape);
                    break;
                case 3:
                    // Draw slightly bigger red circle for power-up
                    powerUpShape.setFillColor(sf::Color::Red);
                    powerUpShape.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - 8, i * CELL_SIZE + CELL_SIZE / 2 - 8);
                    window.draw(powerUpShape);
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
                case -2:
                    // Draw blue rectangle for fixed wall
                    wallShape.setFillColor(sf::Color(100, 10, 255));
                    wallShape.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                    window.draw(wallShape);
                    break;
            }
        }
    }
}


/*
// Function to initialize the game board with values
void initializeGameBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if ((i == 0 || j == 0) || (i == ROWS - 1 || j == COLS - 1)) {
                // Fixed walls
                gameMap[i][j] = -2;
            }
            // Ghost house
            else if((i==10 && j>=10)&&(i==10 && j<=14)){
                if(i!=10 || j!=12)
                gameMap[i][j] = -1;
            }
            else if((i==14 && j>=10)&&(i==14 && j<=14)){
                gameMap[i][j] = -1;
            }
            else if((j==10 && i>=10)&&(j==10 && i<=14)){
                gameMap[i][j] = -1;
            }
            else if((j==14 && i>=10)&&(j==14 && i<=14)){
                gameMap[i][j] = -1;
            }
            // Openings
            else if(i==12 || j==12)
            {
                gameMap[i][j] = 0;
                
            }
            // Small square
            else if((i==6 && j>=6)&&(i==6 && j<=18)){
                gameMap[i][j] = 1;
            }
            else if((i==18 && j>=6)&&(i==18 && j<=18)){
                gameMap[i][j] = 1;
            }
            else if((j==6 && i>=6)&&(j==6 && i<=18)){
                gameMap[i][j] = 1;
            }
            else if((j==18 && i>=6)&&(j==18 && i<=18)){
                gameMap[i][j] = 1;
            }
            // Bigger square
            else if((i==4 && j>=4)&&(i==4 && j<=20)){
                gameMap[i][j] = 1;
            }
            else if((i==20 && j>=4)&&(i==20 && j<=20)){
                gameMap[i][j] = 1;
            }
            else if((j==4 && i>=4)&&(j==4 && i<=20)){
                gameMap[i][j] = 1;
            }
            else if((j==20 && i>=4)&&(j==20 && i<=20)){
                gameMap[i][j] = 1;
            }
            // Bigger square
            else if((i==2 && j>=2)&&(i==2 && j<=22)){
                gameMap[i][j] = 1;
            }
            else if((i==22 && j>=2)&&(i==22 && j<=22)){
                gameMap[i][j] = 1;
            }
            else if((j==2 && i>=2)&&(j==2 && i<=22)){
                gameMap[i][j] = 1;
            }
            else if((j==22 && i>=2)&&(j==22 && i<=22)){
                gameMap[i][j] = 1;
            }
            else 
            {
                // Randomly place pellets and power-ups
                int randNum = rand() % 5; // Generate random number from 0 to 4
                if (randNum == 2) {
                    gameMap[i][j] = 2 + rand() % 3; // Randomly assign power-up value (2, 3, or 4)
                } else {
                    gameMap[i][j] = 0;
                }
            }
        }
    }
}
*/


// Function for the ghost controller thread
/*
void* ghostController(void* arg) {
    // Unpack arguments
    GhostData* ghost = (GhostData*)arg;

    // Ghost controller logic
    while (true) {
        // Determine the direction to move based on the player's position
        pthread_mutex_lock(&SharedmemMutex);
        int playerX = pacman_x; // Example: get the player's x position
        int playerY = pacman_y; // Example: get the player's y position
        pthread_mutex_unlock(&SharedmemMutex);

        int directionX = 0;
        int directionY = 0;

        // Calculate the direction to move
        if (ghost->x < playerX) {
            directionX = 1; // Move right
        } else if (ghost->x > playerX) {
            directionX = -1; // Move left
        }
        if (ghost->y < playerY) {
            directionY = 1; // Move down
        } else if (ghost->y > playerY) {
            directionY = -1; // Move up
        }

        // Update ghost position
        ghost->x += directionX * CELL_SIZE;
        ghost->y += directionY*CELL_SIZE;

        // Sleep for a short duration to control the speed of the ghost
        usleep(200000); // Adjust the sleep duration as needed
    }

    // Return NULL when thread exits
    pthread_exit(NULL);
}
*/


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
        if (nextX >= 0 && nextX < 30 * CELL_SIZE && nextY >= 0 && nextY < 30 * CELL_SIZE &&
            (gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 0 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 2)) {
            ghost->x += directionX * CELL_SIZE;
            ghost->y += directionY * CELL_SIZE;
        }

        //if close
        // Sleep for a short duration to control the speed of the ghost
        if(closwindow == 1)
            {
                pthread_exit(NULL);
            }
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
    if (abs(gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE]) == 1 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == -1 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == -2) {
            cout << "Wall detected!" << endl;
            // set to the old direction

        } 
    else{
    // Update pacman's position
    pacman_x += pacman_direction_x * CELL_SIZE;
    pacman_y += pacman_direction_y * CELL_SIZE;
    }
    
    
}

int main() {
    //XInitThreads();
    // Initialize random seed
    srand(time(nullptr));
    // Initialize game board
    //initializeGameBoard();

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML window");
    // Create the yellow circle (player)
    sf::CircleShape pacman_shape(25/2);
    sf::CircleShape ghost_shape(25/2);
    pacman_shape.setFillColor(sf::Color::Yellow);
    ghost_shape.setFillColor(sf::Color::Red);
    pacman_shape.setPosition(25/8, 25/4); // Set initial position to (100, 50)
    // set ghost position to (10, 10)
    ghost_shape.setPosition(35, 38);
    // Create thread for user input
    pthread_t userInputThread;
    pthread_create(&userInputThread, nullptr, userInput, &window);

    // Initialize ghosts' data
    GhostData ghost1 = {35, 38}; // Example initial position
    // Initialize other ghost data as needed

    // Create threads for ghost controllers
    pthread_t ghost1Thread;
    pthread_create(&ghost1Thread, nullptr, ghostController, &ghost1);
    // Main loop
    while (window.isOpen()) 
    {
        // Clear, draw, and display
        window.clear();
        drawGrid(window);
        movePacman();
        //moveGhost(ghost1);
        pacman_shape.setPosition(pacman_x, pacman_y); // Update pacman position
        ghost_shape.setPosition(ghost1.x, ghost1.y); // Update ghost position
        window.draw(pacman_shape); // Draw the player (yellow circle)
        window.draw(ghost_shape); // Draw the ghost (red circle)
        window.display();
        usleep(150000); // Sleep for 0.3 seconds
        if(closwindow == 1)
        {
            window.close();
        }
        //cout<<"pacman_x: "<<pacman_x<<" pacman_y: "<<pacman_y<<endl;
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



        
