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

// Function to handle user input
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
    pacman_shape.setFillColor(sf::Color::Yellow);
    pacman_shape.setPosition(25/8, 25/4); // Set initial position to (100, 50)

    // Create thread for user input
    pthread_t userInputThread;
    pthread_create(&userInputThread, nullptr, userInput, &window);

    // Create thread for movement
    //pthread_t moveThread;
    //pthread_create(&moveThread, nullptr, reinterpret_cast<void* ()(void)>(movePacman), nullptr);


    // Main loop
    while (window.isOpen()) 
    {
        // Clear, draw, and display
        window.clear();
        drawGrid(window);
        movePacman();
        pacman_shape.setPosition(pacman_x, pacman_y); // Update pacman position
        window.draw(pacman_shape); // Draw the player (yellow circle)
        window.display();
        usleep(150000); // Sleep for 0.3 seconds
        if(closwindow == 1)
        {
            window.close();
        }
    }

    // Join threads
    pthread_join(userInputThread, nullptr);
    //pthread_join(moveThread, nullptr);
    // Destroy mutexes
    pthread_mutex_destroy(&SharedmemMutex);
    //pthread_mutex_destroy(&pacmanMutex);

    return 0;
}