// a star path finding,
// if it cant find the apple find the longest way to the tail

// just chanege vel based on algo
// x_vel = 0;
// y_vel = -1;

// a star algorithm
// starting node and end node
// every field gets a cost. g cost is distance to starting cost. fields nexto each other distance 1 * 10. fields schräg distance √2 = 1.4 * 10
// h cost distance from end node
// f cost g cost + h coast
// go to field with lowest f coast, and explore cost around that field
// keine schrägen nur geraden, da die snake nicht schräg gehen kann
//

// struct g coast, h coast, f coast, explored,

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#define COLS 60
#define ROWS 30
#define MAX_NODES (COLS * ROWS)

typedef struct Position
{
    int x;
    int y;
} Position;

void setup_terminal(struct termios *original_settings);
void restore_terminal(struct termios original_settings);
void placeApple(int *x, int *y, Position *body, int length);
void render_game(int appleX, int appleY, Position *body, int snakeLength, int headX, int headY);
void updateGameState(int *headX, int *headY, int x_vel, int y_vel, int *snakeLength, Position *body, int *appleX, int *appleY, int *newApple, int *running);
void stupidBot(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel);
void a_star(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel);

int main()
{
    // Save original terminal settings
    struct termios original_settings;
    tcgetattr(STDIN_FILENO, &original_settings);
    setup_terminal(&original_settings);

    // Game variables
    int headX = COLS / 2, headY = ROWS / 2;
    int x_vel = 1, y_vel = 0;
    int appleX, appleY;
    int running = 1, newApple = 1, snakeLength = 0;
    Position body[COLS * ROWS];

    while (running)
    {
        render_game(appleX, appleY, body, snakeLength, headX, headY);
        if (newApple)
        {
            placeApple(&appleX, &appleY, body, snakeLength);
            newApple = 0;
        }
        updateGameState(&headX, &headY, x_vel, y_vel, &snakeLength, body, &appleX, &appleY, &newApple, &running);

        // stupidBot(headX, headY, appleX, appleY, &x_vel, &y_vel);
        a_star(headX, headY, appleX, appleY, &x_vel, &y_vel);

        if (!running)
        {
            printf("Game Over!\n");
        }

        // Limit frame rate
        usleep(6 * 1000000 / 60);
    }
    // Restore original terminal settings before exiting
    restore_terminal(original_settings);
    return 0;
}

// a* algorithm

typedef struct Node
{
    Position pos;
    int g_cost;
    int h_cost;
    int f_cost;
} Node;

typedef struct AStarAlgorithm
{
    Node openList[MAX_NODES];
    Node closedList[MAX_NODES];
    int openListSize;
    int closedListSize;
} AStarAlgorithm;

void stupidBot(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel)
{
    if (headY > appleY)
    {
        *x_vel = 0;
        *y_vel = -1; // Move up
    }
    else if (headY < appleY)
    {
        *x_vel = 0;
        *y_vel = 1; // Move down
    }
    else if (headX > appleX)
    {
        *x_vel = -1;
        *y_vel = 0; // Move left
    }
    else if (headX < appleX)
    {
        *x_vel = 1;
        *y_vel = 0; // Move right
    }
}

void a_star(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel)
{
    Position startNode = {headX, headY};
    Position endNode = {appleX, appleY};
}