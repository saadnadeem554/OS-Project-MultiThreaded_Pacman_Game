#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <unistd.h>
#include <iostream>
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







// Define a structure to represent a cell in the grid
struct Cell {
    int x, y;
    int distance;
    bool operator<(const Cell &other) const {
        return distance > other.distance; // For min-heap
    }
};

// Define helper function to calculate Manhattan distance
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Dijkstra's algorithm function
int** dijkstra(int srcX, int srcY, int destX, int destY, const int gameMap[ROWS][COLS]) {
    // Initialize distances to infinity
    int** dist = new int*[ROWS];
    for (int i = 0; i < ROWS; ++i) {
        dist[i] = new int[COLS];
        for (int j = 0; j < COLS; ++j) {
            dist[i][j] = INT_MAX;
        }
    }

    // Initialize priority queue
    priority_queue<Cell> pq;

    // Push source cell into priority queue
    pq.push({srcX, srcY, 0});
    dist[srcX][srcY] = 0;

    // Define possible movements
    int dx[] = {0, 0, -1, 1}; // Up, down, left, right
    int dy[] = {-1, 1, 0, 0};

    while (!pq.empty()) {
        Cell curr = pq.top();
        pq.pop();

        if (curr.x == destX && curr.y == destY) {
            // Found destination, stop search
            break;
        }

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int newX = curr.x + dx[i];
            int newY = curr.y + dy[i];

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && gameMap[newY / CELL_SIZE][newX / CELL_SIZE] != 1 && dist[newX][newY] > curr.distance + 1) 
            {
                dist[newX][newY] = curr.distance + 1;
                pq.push({newX, newY, curr.distance + 1});
            }
        }
    }

    return dist;
}





void moveGhost(GhostData &ghost) {
    // Use Dijkstra's algorithm to find the shortest path to pacman
    int** shortestPath = dijkstra(ghost.x, ghost.y, pacman_x, pacman_y, gameMap);
    
    // Get the next cell in the shortest path
    int nextX = pacman_x;
    int nextY = pacman_y;

    // Update ghost direction based on the next cell
    if (nextX < ghost.x) {
        ghost.direction = 3; // Left
    } else if (nextX > ghost.x) {
        ghost.direction = 4; // Right
    } else if (nextY < ghost.y) {
        ghost.direction = 1; // Up
    } else if (nextY > ghost.y) {
        ghost.direction = 2; // Down
    }

    // Move the ghost to the next cell
    ghost.x = nextX;
    ghost.y = nextY;

    // Free memory allocated for the shortest path
    for (int i = 0; i < ROWS; ++i) {
        delete[] shortestPath[i];
    }
    delete[] shortestPath;
}


// Ghost controller function
void *ghostController2(void *arg) {
    // Unpack arguments
    GhostData *ghost = (GhostData *)arg;

    // Ghost controller logic
    while (true) {
        // Move ghost
        moveGhost(*ghost);

        // Sleep for a short duration to control the speed of the ghost
        usleep(200000); // Adjust the sleep duration as needed
    }

    pthread_exit(NULL);
}



















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



int bfs_shortest_distance(int grid[][30], pair<int, int> start_pos, pair<int, int> end_pos) {
    int rows = 30;
    int cols = 30;
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<int>> distance(rows, vector<int>(cols, INT_MAX)); // Initialize distances to infinity

    queue<pair<int, int>> q;
    q.push(start_pos);
    visited[start_pos.first][start_pos.second] = true;
    distance[start_pos.first][start_pos.second] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        pair<int, int> current_pos = q.front();
        q.pop();

        if (current_pos == end_pos) {
            return distance[current_pos.first][current_pos.second]; // Return distance when destination is reached
        }

        for (int j = 0; j < 4; ++j) {
            int new_row = current_pos.first + dr[j];
            int new_col = current_pos.second + dc[j];
            if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols && (grid[new_row][new_col] == 2 || grid[new_row][new_col] == 0) && !visited[new_row][new_col]) {
                visited[new_row][new_col] = true;
                distance[new_row][new_col] = distance[current_pos.first][current_pos.second] + 1;
                q.push({new_row, new_col});
            }
        }
    }

    return INT_MAX; // No path found
}

bool isValid(int row, int col, int rows, int cols, vector<vector<int>>& grid, vector<vector<bool>>& visited) {
    return (row >= 0 && row < rows && col >= 0 && col < cols && (grid[row][col] == 2 || grid[row][col] == 0) && !visited[row][col]);
}

int findShortestDistance(int grid[][30], pair<int, int> start_pos, pair<int, int> end_pos) {
    int rows = 30;
    int cols = 30;
    vector<vector<int>> gridVec(rows, vector<int>(cols, 0));
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            gridVec[i][j] = grid[i][j];
        }
    }

    stack<pair<int, int>> st;
    st.push(start_pos);
    visited[start_pos.first][start_pos.second] = true;

    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!st.empty()) {
        pair<int, int> current_pos = st.top();
        st.pop();

        if (current_pos == end_pos) {
            return visited[current_pos.first][current_pos.second] - 1; // Return distance when destination is reached
        }

        for (auto dir : directions) {
            int new_row = current_pos.first + dir.first;
            int new_col = current_pos.second + dir.second;
            if (isValid(new_row, new_col, rows, cols, gridVec, visited)) {
                visited[new_row][new_col] = visited[current_pos.first][current_pos.second] + 1;
                st.push({new_row, new_col});
            }
        }
    }

    return -1; // No path found
}
void* ghostController3(void* arg) {
    GhostData* ghost = (GhostData*)arg;
    while(true)
    {
        int current_posx = ghost->x;
        int current_posy = ghost->y;

      int shortest_distance1=222220; 
      int shortest_distance2=222220;
      int shortest_distance3=222220;        //usleep(200000); // Adjust the sleep duration as needed

      int shortest_distance4=222220;  
        if(gameMap[current_posy / CELL_SIZE][((current_posx ) / CELL_SIZE)+1]==0 || gameMap[current_posy / CELL_SIZE][((current_posx ) / CELL_SIZE)+ 1]==2)
        {
            cout<<current_posx / CELL_SIZE + 1<<" "<<endl;
            cout<<"Grid val"<<gameMap[current_posy / CELL_SIZE][((current_posx ) / CELL_SIZE)+1]<<endl;
            pair<int, int> start_pos = {current_posx / CELL_SIZE + 1 , (current_posy/ CELL_SIZE)};
            pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};
             shortest_distance1 = findShortestDistance(gameMap, start_pos, end_pos);
             cout<<"right"<<endl;
        }
        if(gameMap[current_posy / CELL_SIZE][current_posx  / CELL_SIZE - 1]==0 || gameMap[current_posy / CELL_SIZE][current_posx  / CELL_SIZE - 1]==2)
        {
            pair<int, int> start_pos = {current_posx / CELL_SIZE -1, (current_posy/ CELL_SIZE)};
            pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};

            shortest_distance2 = findShortestDistance(gameMap, start_pos, end_pos);
            cout<<"lrft"<<endl;

        }
        if(gameMap[current_posy / CELL_SIZE - 1  ][(current_posx) / CELL_SIZE]==0 || gameMap[current_posy / CELL_SIZE - 1  ][(current_posx) / CELL_SIZE]==2)
        {
              
            pair<int, int> start_pos = {(current_posx / CELL_SIZE), ((current_posy )/ CELL_SIZE - 1)};
            pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};
            shortest_distance3 = findShortestDistance(gameMap, start_pos, end_pos);
            cout<<"up"<<endl;

        }
        if(gameMap[current_posy / CELL_SIZE  + 1][(current_posx) / CELL_SIZE]==0 || gameMap[current_posy / CELL_SIZE  + 1][(current_posx) / CELL_SIZE]==2)
        {
            pair<int, int> start_pos = {(current_posx / CELL_SIZE), ((current_posy)/ CELL_SIZE) + 1};
            pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};
             shortest_distance4 = findShortestDistance(gameMap, start_pos, end_pos);
            cout<<"down"<<endl;

        }
            cout<<shortest_distance1 <<" " << shortest_distance2 << " "<< shortest_distance3 << " "<< shortest_distance4 <<endl ; 
            int final = min (shortest_distance1, min(shortest_distance2 ,min(shortest_distance4 , shortest_distance3 ))); 
    
        if (final  != 222220) {
        cout << "Shortest distance: " << final << endl;
    } else {
        cout << "No path found" << endl;
    }
    int directionX = 0;
    int directionY = 0;
    if(final==shortest_distance1 && final!= 222220) {
        directionX = 1;
        ghost->direction = 4;
    }
    else if(final == shortest_distance2 && final!= 222220){
        directionX = -1;
        ghost->direction = 3;
    }
     else if (final == shortest_distance3 && final!= 222220){
        directionY = -1;
        ghost->direction = 1;
    }
    else if ( final == shortest_distance4 && final!= 222220){
        directionY = 1;
        ghost->direction = 2;
    }

    ghost->x += directionX * CELL_SIZE;
    ghost->y += directionY * CELL_SIZE;
    usleep(200000); // Adjust the sleep duration as needed

    }
    pthread_exit(NULL);
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

    pthread_create(&ghost1Thread, nullptr, ghostController3, &ghost1);
    //pthread_create(&ghost2Thread, nullptr, ghostController, &ghost2);
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



        
