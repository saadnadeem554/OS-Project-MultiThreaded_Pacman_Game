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

int pacman_x = CELL_SIZE + 25 / 8;
int pacman_y = CELL_SIZE + 25 / 4;
sf::Clock powerupClock ; 

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
bool powerupActive = false;
// shared memory of user direcion
struct shared
{
    int pacman_direction = 0;
} sharedmem;
int closwindow = 0;

// Ghst structure
struct GhostData
{
    int x;
    int y;
    int direction = 2;
    // Add any other relevant data here
};

// Define a structure to represent a cell in the grid
struct Cell
{
    int x, y;
    int distance;
    bool operator<(const Cell &other) const
    {
        return distance > other.distance; // For min-heap
    }
};

// Define helper function to calculate Manhattan distance
int manhattanDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// Dijkstra's algorithm function
int **dijkstra(int srcX, int srcY, int destX, int destY, const int gameMap[ROWS][COLS])
{
    // Initialize distances to infinity
    int **dist = new int *[ROWS];
    for (int i = 0; i < ROWS; ++i)
    {
        dist[i] = new int[COLS];
        for (int j = 0; j < COLS; ++j)
        {
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

    while (!pq.empty())
    {
        Cell curr = pq.top();
        pq.pop();

        if (curr.x == destX && curr.y == destY)
        {
            // Found destination, stop search
            break;
        }

        // Explore neighbors
        for (int i = 0; i < 4; ++i)
        {
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

void moveGhost(GhostData &ghost)
{
    // Use Dijkstra's algorithm to find the shortest path to pacman
    int **shortestPath = dijkstra(ghost.x, ghost.y, pacman_x, pacman_y, gameMap);

    // Get the next cell in the shortest path
    int nextX = pacman_x;
    int nextY = pacman_y;

    // Update ghost direction based on the next cell
    if (nextX < ghost.x)
    {
        ghost.direction = 3; // Left
    }
    else if (nextX > ghost.x)
    {
        ghost.direction = 4; // Right
    }
    else if (nextY < ghost.y)
    {
        ghost.direction = 1; // Up
    }
    else if (nextY > ghost.y)
    {
        ghost.direction = 2; // Down
    }

    // Move the ghost to the next cell
    ghost.x = nextX;
    ghost.y = nextY;

    // Free memory allocated for the shortest path
    for (int i = 0; i < ROWS; ++i)
    {
        delete[] shortestPath[i];
    }
    delete[] shortestPath;
}

// Ghost controller function
void *ghostController2(void *arg)
{
    // Unpack arguments
    GhostData *ghost = (GhostData *)arg;

    // Ghost controller logic
    while (true)
    {
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
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            gameMap[i][j] = 0;
        }
    }
    // in the gameMapSkeleton wherever there is a 0 put a score pallet there
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (gameMapSkeleton[i][j] == 0)
            {
                gameMap[i][j] = 2;
                totalScorePallet++;
            }
            else
                gameMap[i][j] = gameMapSkeleton[i][j];
        }
    }
}

// Function to initialize the game board with values
void initializeGameBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if ((i == 0 || j == 0) || (i == ROWS - 1 || j == COLS - 1))
            {
                // Fixed walls
                gameMap[i][j] = -2;
            }
            // Ghost house
            else if ((i == 10 && j >= 10) && (i == 10 && j <= 14))
            {
                if (i != 10 || j != 12)
                    gameMap[i][j] = -1;
            }
            else if ((i == 14 && j >= 10) && (i == 14 && j <= 14))
            {
                gameMap[i][j] = -1;
            }
            else if ((j == 10 && i >= 10) && (j == 10 && i <= 14))
            {
                gameMap[i][j] = -1;
            }
            else if ((j == 14 && i >= 10) && (j == 14 && i <= 14))
            {
                gameMap[i][j] = -1;
            }
            // Openings
            else if (i == 12 || j == 12)
            {
                gameMap[i][j] = 0;
            }
            // Small square
            else if ((i == 6 && j >= 6) && (i == 6 && j <= 18))
            {
                gameMap[i][j] = 1;
            }
            else if ((i == 18 && j >= 6) && (i == 18 && j <= 18))
            {
                gameMap[i][j] = 1;
            }
            else if ((j == 6 && i >= 6) && (j == 6 && i <= 18))
            {
                gameMap[i][j] = 1;
            }
            else if ((j == 18 && i >= 6) && (j == 18 && i <= 18))
            {
                gameMap[i][j] = 1;
            }
            // Bigger square
            else if ((i == 4 && j >= 4) && (i == 4 && j <= 20))
            {
                gameMap[i][j] = 1;
            }
            else if ((i == 20 && j >= 4) && (i == 20 && j <= 20))
            {
                gameMap[i][j] = 1;
            }
            else if ((j == 4 && i >= 4) && (j == 4 && i <= 20))
            {
                gameMap[i][j] = 1;
            }
            else if ((j == 20 && i >= 4) && (j == 20 && i <= 20))
            {
                gameMap[i][j] = 1;
            }
            // Bigger square
            else if ((i == 2 && j >= 2) && (i == 2 && j <= 22))
            {
                gameMap[i][j] = 1;
            }
            else if ((i == 22 && j >= 2) && (i == 22 && j <= 22))
            {
                gameMap[i][j] = 1;
            }
            else if ((j == 2 && i >= 2) && (j == 2 && i <= 22))
            {
                gameMap[i][j] = 1;
            }
            else if ((j == 22 && i >= 2) && (j == 22 && i <= 22))
            {
                gameMap[i][j] = 1;
            }
            else
            {
                // Randomly place pellets and power-ups
                int randNum = rand() % 5; // Generate random number from 0 to 4
                if (randNum == 2)
                {
                    gameMap[i][j] = 2 + rand() % 3; // Randomly assign power-up value (2, 3, or 4)
                }
                else
                {
                    gameMap[i][j] = 0;
                }
            }
        }
    }
}

// Function to draw the grid with appropriate shapes for pellets, power-ups, and walls
void drawGrid(sf::RenderWindow &window)
{
    sf::CircleShape pelletShape(5);
    sf::CircleShape powerUpShape(5);
    sf::RectangleShape wallShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)); // wall thickness = 1
    sf::Color darkBlue(0, 0, 139);
    sf::Text scoreText;
    sf::Font font;
    font.loadFromFile("SIXTY.TTF");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(CELL_SIZE * 10, CELL_SIZE * 30);
    scoreText.setString("Score: " + std::to_string(Score));
    window.draw(scoreText);

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
            case 4:
                // cout<<i<<" "<<j <<endl ;
                //  Draw blue rectangle for wall
                powerUpShape.setFillColor(sf::Color::Red);
                powerUpShape.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - 5, i * CELL_SIZE + CELL_SIZE / 2 - 5);
                window.draw(powerUpShape);
                break;
            }
            // unlock mutex
            pthread_mutex_unlock(&GameBoardMutex);
        }
    }
}

struct Node
{
    pair<int, int> pos;
    int g_score;
    int h_score;
    Node *parent;

    Node(pair<int, int> pos, int g_score, int h_score, Node *parent) : pos(pos), g_score(g_score), h_score(h_score), parent(parent) {}

    int f_score() const
    {
        return g_score + h_score;
    }
};

int manhattan_distance(pair<int, int> a, pair<int, int> b)
{
    return abs(a.first - b.first) + abs(a.second - b.second);
}

bool isValid(int row, int col, int grid[][30])
{
    return (row >= 0 && col >= 0 && (grid[row][col] == 2 || grid[row][col] == 0));
}

vector<pair<int, int>> getNeighbors(pair<int, int> pos, int rows, int cols)
{
    vector<pair<int, int>> neighbors;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i)
    {
        int new_row = pos.first + dr[i];
        int new_col = pos.second + dc[i];
        if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols)
        {
            neighbors.push_back({new_row, new_col});
        }
    }

    return neighbors;
}

int bfs_shortest_distance(int grid[][30], pair<int, int> start_pos, pair<int, int> end_pos, pair<int, int> called_pos)
{
    int rows = 30;
    int cols = 30;
    // cout<<"start_pos"<<start_pos.first<<" "<<start_pos.second<<endl;
    // vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    bool visited[30][30];
    // vector<vector<int>> distance(rows, vector<int>(cols, INT_MAX)); // Initialize distances to infinity
    int distance[30][30];
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 1 || grid[i][j] == -1)
            {
                visited[i][j] = true;
                distance[i][j] = 0;
            }
            else
            {
                visited[i][j] = false;
                distance[i][j] = INT_MAX;
            }
        }
    }

    queue<pair<int, int>> q;
    q.push(start_pos);
    visited[start_pos.first][start_pos.second] = true;
    visited[called_pos.first][called_pos.second] = true;
    distance[start_pos.first][start_pos.second] = 0;
    distance[called_pos.first][called_pos.second] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty())
    {
        pair<int, int> current_pos = q.front();
        q.pop();
        if (current_pos == end_pos)
        {
            return distance[current_pos.first][current_pos.second]; // Return distance when destination is reached
        }

        for (int j = 0; j < 4; ++j)
        {
            int new_row = current_pos.first + dr[j];
            int new_col = current_pos.second + dc[j];
            // cout<<new_row<<" "<<new_col<<endl;
            // cout<<visited[new_row][new_col]<<endl;
            if (new_row > 0 && new_row < rows && new_col > 0 && new_col < cols && (grid[new_row][new_col] == 2 || grid[new_row][new_col] == 0) && !visited[new_row][new_col])
            {
                visited[new_row][new_col] = true;
                distance[new_row][new_col] = distance[current_pos.first][current_pos.second] + 1;
                q.push({new_row, new_col});
            }
        }
    }

    return INT_MAX; // No path found
}

bool isValid(int row, int col, vector<vector<int>> &grid)
{
    return (row >= 0 && col >= 0 && (grid[row][col] == 2 || grid[row][col] == 0));
}

int bfs_encountered_positions(int grid[][30], pair<int, int> start_pos, pair<int, int> end_pos)
{
    int rows = 30;
    int cols = 30;
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 1 || grid[i][j] == -1)
                visited[i][j] = true;
        }
    }

    queue<pair<int, int>> q;
    q.push(start_pos);
    visited[start_pos.first][start_pos.second] = true;

    int encountered_positions = 0;

    while (!q.empty())
    {
        pair<int, int> current_pos = q.front();
        q.pop();

        // Check if we've reached the end position
        if (current_pos == end_pos)
        {
            return encountered_positions;
        }

        // Define the directions (up, down, left, right)
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        // Explore neighbors
        for (int i = 0; i < 4; ++i)
        {
            int new_row = current_pos.first + dr[i];
            int new_col = current_pos.second + dc[i];
            if (new_row > 0 && new_row < rows && new_col > 0 && new_col < cols &&
                (grid[new_row][new_col] == 2 || grid[new_row][new_col] == 0) && !visited[new_row][new_col])
            {
                visited[new_row][new_col] = true;
                // Increment the encountered positions count
                encountered_positions++;
                q.push({new_row, new_col});
            }
        }
    }

    // If end position is not reached, return -1
    return INT_MAX;
}

int shortestPath(int startX, int startY, int destX, int destY, int gameMap[ROWS][COLS])
{
    bool visited[ROWS][COLS] = {false};
    std::queue<std::pair<int, int>> q;
    q.push({startX, startY});
    visited[startX][startY] = true;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    int dist[ROWS][COLS] = {0}; // Store distances from start position

    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // Check if destination is reached
        if (x == destX && y == destY)
        {
            return dist[x][y];
        }

        // Explore adjacent cells
        for (int i = 0; i < 4; ++i)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            // Check validity and unvisited status
            if (isValid(newX, newY, gameMap) && !visited[newX][newY])
            {
                q.push({newX, newY});
                visited[newX][newY] = true;
                dist[newX][newY] = dist[x][y] + 1; // Update distance
            }
        }
    }

    // If destination is unreachable
    return INT_MAX;
}

void *ghostController3(void *arg)
{
    GhostData *ghost = (GhostData *)arg;
    while (true)
    {
        cout << ghost->x << " " << ghost->y << endl;
        int current_posx = ghost->x;
        int current_posy = ghost->y;

        int shortest_distance1 = 222220;
        int shortest_distance2 = 222220;
        int shortest_distance3 = 222220; // usleep(200000); // Adjust the sleep duration as needed

        int shortest_distance4 = 222220;
        if (gameMap[current_posy / CELL_SIZE][((current_posx) / CELL_SIZE) + 1] == 0 || gameMap[current_posy / CELL_SIZE][((current_posx) / CELL_SIZE) + 1] == 2)
        {
            // cout<<current_posx / CELL_SIZE + 1<<" "<<endl;
            // cout<<"Grid val"<<gameMap[current_posy / CELL_SIZE][((current_posx ) / CELL_SIZE)+1]<<endl;
            // pair<int, int> start_pos = {current_posx / CELL_SIZE + 1 , (current_posy/ CELL_SIZE)};
            // pair<int, int> called_pos = {current_posx / CELL_SIZE , (current_posy/ CELL_SIZE)};
            // pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};

            // shortest_distance1 = bfs_shortest_distance(gameMap, start_pos, end_pos, called_pos);
            // shortest_distance1 = bfs_encountered_positions(gameMap, start_pos, end_pos);
            // cout<<"right"<<endl;
            int startX = ghost->x / CELL_SIZE + 1;
            int startY = ghost->y / CELL_SIZE;
            int destX = pacman_x / CELL_SIZE;
            int destY = pacman_y / CELL_SIZE;
            shortest_distance1 = shortestPath(startX, startY, destX, destY, gameMap);
        }
        if (gameMap[current_posy / CELL_SIZE][current_posx / CELL_SIZE - 1] == 0 || gameMap[current_posy / CELL_SIZE][current_posx / CELL_SIZE - 1] == 2)
        {
            // pair<int, int> start_pos = {current_posx / CELL_SIZE -1, (current_posy/ CELL_SIZE)};
            // pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};
            // pair<int, int> called_pos = {current_posx / CELL_SIZE , (current_posy/ CELL_SIZE)};
            // shortest_distance2 = bfs_shortest_distance(gameMap, start_pos, end_pos, called_pos);
            // shortest_distance2 = bfs_encountered_positions(gameMap, start_pos, end_pos);
            cout << "lrft" << endl;

            int startX = ghost->x / CELL_SIZE - 1;
            int startY = ghost->y / CELL_SIZE;
            int destX = pacman_x / CELL_SIZE;
            int destY = pacman_y / CELL_SIZE;
            shortest_distance2 = shortestPath(startX, startY, destX, destY, gameMap);
        }
        if (gameMap[current_posy / CELL_SIZE - 1][(current_posx) / CELL_SIZE] == 0 || gameMap[current_posy / CELL_SIZE - 1][(current_posx) / CELL_SIZE] == 2)
        {

            // pair<int, int> start_pos = {(current_posx / CELL_SIZE), ((current_posy )/ CELL_SIZE - 1)};
            // pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};
            // pair<int, int> called_pos = {current_posx / CELL_SIZE , (current_posy/ CELL_SIZE)};
            // shortest_distance3 = bfs_shortest_distance(gameMap, start_pos, end_pos,called_pos);
            // shortest_distance3 = bfs_encountered_positions(gameMap, start_pos, end_pos);
            cout << "up" << endl;
            int startX = ghost->x / CELL_SIZE;
            int startY = ghost->y / CELL_SIZE - 1;
            int destX = pacman_x / CELL_SIZE;
            int destY = pacman_y / CELL_SIZE;
            shortest_distance3 = shortestPath(startX, startY, destX, destY, gameMap);
        }
        if (gameMap[current_posy / CELL_SIZE + 1][(current_posx) / CELL_SIZE] == 0 || gameMap[current_posy / CELL_SIZE + 1][(current_posx) / CELL_SIZE] == 2)
        {
            // pair<int, int> start_pos = {(current_posx / CELL_SIZE), ((current_posy)/ CELL_SIZE) + 1};
            // pair<int, int> end_pos = {pacman_x/CELL_SIZE, pacman_y/CELL_SIZE};
            // pair<int, int> called_pos = {current_posx / CELL_SIZE , (current_posy/ CELL_SIZE)};

            // shortest_distance4 = bfs_shortest_distance(gameMap, start_pos, end_pos, called_pos);
            // shortest_distance4 = bfs_encountered_positions(gameMap, start_pos, end_pos);
            cout << "down" << endl;
            int startX = ghost->x / CELL_SIZE;
            int startY = ghost->y / CELL_SIZE + 1;
            int destX = pacman_x / CELL_SIZE;
            int destY = pacman_y / CELL_SIZE;
            shortest_distance1 = shortestPath(startX, startY, destX, destY, gameMap);
        }
        cout << shortest_distance1 << " " << shortest_distance2 << " " << shortest_distance3 << " " << shortest_distance4 << endl;
        int final = min(shortest_distance1, min(shortest_distance2, min(shortest_distance4, shortest_distance3)));

        // if (final  != 222220) {
        // cout << "Shortest distance: " << final << endl;
        //} else {
        //    cout << "No path found" << endl;
        //}
        int directionX = 0;
        int directionY = 0;
        if (final == shortest_distance1 && final != 222220)
        {
            directionX = 1;
            ghost->direction = 4;
        }
        else if (final == shortest_distance2 && final != 222220)
        {
            directionX = -1;
            ghost->direction = 3;
        }
        else if (final == shortest_distance3 && final != 222220)
        {
            directionY = -1;
            ghost->direction = 1;
        }
        else if (final == shortest_distance4 && final != 222220)
        {
            directionY = 1;
            ghost->direction = 2;
        }

        ghost->x = ghost->x + directionX * CELL_SIZE;
        ghost->y = ghost->y + directionY * CELL_SIZE;
        // cout<<ghost->x<<" "<<ghost->y<<endl;
        usleep(100000); // Adjust the sleep duration as needed
    }
    pthread_exit(NULL);
}

// User
void *ghostController(void *arg)
{
    // Unpack arguments
    GhostData *ghost = (GhostData *)arg;

    // Ghost controller logic
    while (true)
    {
        int directionX = 0;
        int directionY = 0;

        bool chosen = false;
        pthread_mutex_lock(&GameBoardMutex);
       
        
        

            if (ghost->direction == 1 || ghost->direction == 2)
            { // Up or down
                if (rand() % 2 == 0)
                { // Randomly choose left or right
                    if (gameMap[ghost->y / CELL_SIZE][(ghost->x + CELL_SIZE) / CELL_SIZE] != 1 &&
                        gameMap[ghost->y / CELL_SIZE][(ghost->x + CELL_SIZE) / CELL_SIZE] != -1 &&
                        gameMap[ghost->y / CELL_SIZE][(ghost->x + CELL_SIZE) / CELL_SIZE] != -2)
                    {


                        directionX = 1;
                        chosen = true;
                        ghost->direction = 4;
                    }
                }
                else
                {
                    if (gameMap[ghost->y / CELL_SIZE][(ghost->x - CELL_SIZE) / CELL_SIZE] != 1 &&
                        gameMap[ghost->y / CELL_SIZE][(ghost->x - CELL_SIZE) / CELL_SIZE] != -1 &&
                        gameMap[ghost->y / CELL_SIZE][(ghost->x - CELL_SIZE) / CELL_SIZE] != -2)
                    {

                        directionX = -1;
                        chosen = true;
                        ghost->direction = 3;
                    }
                }
            }
            else if (ghost->direction == 3 || ghost->direction == 4)
            { // Left or right
                if (rand() % 2 == 0)
                { // Randomly choose up or down
                    if (gameMap[(ghost->y + CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != 1 &&
                        gameMap[(ghost->y + CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -1 &&
                        gameMap[(ghost->y + CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -2)
                    {   

                        directionY = 1;
                        chosen = true;
                        ghost->direction = 1;
                       
                    }
                }
                else
                {
                    if (gameMap[(ghost->y - CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != 1 &&
                        gameMap[(ghost->y - CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -1 &&
                        gameMap[(ghost->y - CELL_SIZE) / CELL_SIZE][(ghost->x) / CELL_SIZE] != -2)
                    {

                        directionY = -1;
                        chosen = true;
                        ghost->direction = 2;
                        
                    }
                }
            }
        

        pthread_mutex_unlock(&GameBoardMutex);
        if (!chosen)
        {
            // Continue in current direction
            if (ghost->direction == 1)
            {
                directionY = 1;
            }
            else if (ghost->direction == 2)
            {
                directionY = -1;
            }
            else if (ghost->direction == 3)
            {
                directionX = -1;
            }
            else
            {
                directionX = 1;
            }
        }

        int nextX = ghost->x + directionX * CELL_SIZE;
        int nextY = ghost->y + directionY * CELL_SIZE;

        // Check for collisions with walls
        pthread_mutex_lock(&GameBoardMutex);
        if (nextX > 0 && nextX < 30 * CELL_SIZE && nextY > 0 && nextY < 30 * CELL_SIZE && (gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 0 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 2))
        {
            ghost->x += directionX * CELL_SIZE;
            ghost->y += directionY * CELL_SIZE;
                        cout<<ghost->x/CELL_SIZE<<" "<< ghost->y/CELL_SIZE<<endl; 

        }
        else
        {
            // move ghost in opposite direction
            cout<<"walled";
            if (ghost->direction == 1)
            {
                ghost->direction = 2;
                directionY = 1 ; 
                ghost->x += directionX * CELL_SIZE;
                ghost->y += directionY * CELL_SIZE;
            }
            else if (ghost->direction == 2)
            {
                ghost->direction = 1;
                directionY =-1 ; 

                ghost->x += directionX * CELL_SIZE;
                ghost->y += directionY * CELL_SIZE;
            }
            else if (ghost->direction == 3)
            {
                ghost->direction = 4;
                directionX = 1 ; 
                ghost->x += directionX * CELL_SIZE;
                ghost->y += directionY * CELL_SIZE;
            }
            else
            {
                ghost->direction = 3;
                directionX = -1 ; 
                ghost->x += directionX * CELL_SIZE;
                ghost->y += directionY * CELL_SIZE;
            }
            //sleep(1);
            //cout<<ghost->x/CELL_SIZE<<" "<< ghost->y/CELL_SIZE<<endl; 
        }
        pthread_mutex_unlock(&GameBoardMutex);

        // if close
        if (closwindow == 1)
        {
            pthread_exit(NULL);
        }
        // Sleep for a short duration to control the speed of the ghost
        usleep(200000); // Adjust the sleep duration as needed
    }
    pthread_exit(NULL);
}

// Function to handle user input (UI TREAD)
void *userInput(void *arg)
{
    // Unpack arguments
    sf::RenderWindow *window = (sf::RenderWindow *)arg;
    while (window->isOpen())
    {
        Event event;
        // Process SFML events
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
            {

                closwindow = 1;
                // window->close();
                pthread_exit(NULL);
                // exit(1);
            }
            else if (event.type == Event::KeyPressed)
            {
                // Lock mutex before accessing shared variable
                pthread_mutex_lock(&SharedmemMutex);
                if (event.key.code == Keyboard::Up)
                {
                    sharedmem.pacman_direction = 1;
                }
                else if (event.key.code == Keyboard::Down)
                {
                    sharedmem.pacman_direction = 2;
                }
                else if (event.key.code == Keyboard::Left)
                {
                    sharedmem.pacman_direction = 3;
                }
                else if (event.key.code == Keyboard::Right)
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
    switch (pacdirection)
    {
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
    if (abs(gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE]) == 1 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == -1 || gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == -2)
    {
        // cout << "Wall detected!" << endl;
        // set to the old direction
    }
    else
    {
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
        // cout<<Score<<endl;
    }
    if (abs(gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] == 4) &&  powerupActive == false)
    {
        gameMap[nextY / CELL_SIZE][nextX / CELL_SIZE] = 0;
        powerupActive = true;
        if (powerupActive)
           {
            cout << "Eaten power up ";
            powerupClock.restart();
            // make a new thread and let it sleep for 5 sec , then change the flag 

           }

        

        // maybe start a new thread for 5 seconds and then change the flag back to false ;
    }
    
    


    pthread_mutex_unlock(&GameBoardMutex);
}

int main()
{

    // XInitThreads();
    //  Initialize random seed
    srand(time(nullptr));
    // Initialize game board
    intitializeGrid();
    int count = 0; // Counter for the number of positions initialized with 4
    // two random posiiton for red circle (power up value  = 4 )
    while (count < 2)
    {
        int randRow = rand() % ROWS;
        int randCol = rand() % COLS;
        if (gameMap[randRow][randCol] == 2)
        {
            gameMap[randRow][randCol] = 4;
            count++;
        }
    }

    // initializeGameBoard();
    //  Create SFML window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML window");
    // Create the yellow circle (player)
    sf::CircleShape pacman_shape(25 / 2);
    sf::CircleShape ghost_shape1(25 / 2);
    sf::CircleShape ghost_shape2(25 / 2);

    pacman_shape.setFillColor(sf::Color::Yellow);
    ghost_shape1.setFillColor(sf::Color::Red);
    ghost_shape2.setFillColor(sf::Color::Blue);
    pacman_shape.setPosition(25 / 8, 25 / 4); // Set initial position to (100, 50)
    // set ghost position to (10, 10)
    ghost_shape1.setPosition(65, 38);
    ghost_shape2.setPosition(65, 38);

    // Create thread for user input
    pthread_t userInputThread;
    pthread_create(&userInputThread, nullptr, userInput, &window);

    // Initialize ghosts' data
    GhostData ghost1 = {899, 38}; // Example initial position
    GhostData ghost2 = {65, 38};  // Example initial position
    // ghost2.direction = 3;

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

        if(powerupActive )
        {
             ghost_shape1.setFillColor(sf::Color::White);
             ghost_shape2.setFillColor(sf::Color :: White);
        }
        if(powerupClock.getElapsedTime().asSeconds() >=5 )
        {
            powerupActive = false ; 
        //    cout<<"Removed power up ; ";
            ghost_shape1.setFillColor(sf::Color::Red);
             ghost_shape2.setFillColor(sf::Color :: Blue);
        }
        if( powerupActive   &&  pacman_x ==  ghost1.x && pacman_y == ghost1.y)
        {
            ghost1.x = 65;
            ghost1.y = 38;
            Score = Score + 10 ;

        }
        if(powerupActive &&  pacman_x == ghost2.x &&  pacman_y == ghost2.y)
        {
            ghost2.x = 65;
            ghost2.y = 38;
            Score = Score + 10 ;
        }
        movePacman();
        window.clear();
        drawGrid(window);

        // moveGhost(ghost1);
        pacman_shape.setPosition(pacman_x, pacman_y); // Update pacman position
        ghost_shape1.setPosition(ghost1.x, ghost1.y); // Update ghost position
        ghost_shape2.setPosition(ghost2.x, ghost2.y); // Update ghost position
        window.draw(pacman_shape);                    // Draw the player (yellow circle)
        window.draw(ghost_shape1);                    // Draw the ghost (red circle)
        window.draw(ghost_shape2);
        window.display();
        usleep(150000); // Sleep for 0.3 seconds
        if (closwindow == 1)
        {
            window.close();
        }
        // cout<<"pacman_x: "<<pacman_x<<" pacman_y: "<<pacman_y<<endl;
    }

    // Join threads
    pthread_join(userInputThread, nullptr);
    pthread_join(ghost1Thread, nullptr);
    pthread_join(ghost2Thread, nullptr);
    // pthread_join(moveThread, nullptr);
    //  Destroy mutexes
    pthread_mutex_destroy(&SharedmemMutex);
    // pthread_mutex_destroy(&pacmanMutex);

    return 0;
}
