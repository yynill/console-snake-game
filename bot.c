#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <math.h>

#include "my_structs.h"

// functions from a_star.c
void setup_terminal(struct termios *original_settings);
void restore_terminal(struct termios original_settings);
void placeApple(int *x, int *y, Position *body, int length);
void render_game(int appleX, int appleY, Position *body, int snakeLength, int headX, int headY, NodeList bPath);
void updateGameState(int *headX, int *headY, int x_vel, int y_vel, int *snakeLength, Position *body, int *appleX, int *appleY, int *newApple, int *running);
void init_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel, Position body[], int snakeLength);
NodeList run_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel);

void adjustVelocity(int *x_vel, int *y_vel, int headX, int headY, int appleX, int appleY, NodeList bPath);

int main()
{
    // game variables
    int headX = COLS / 2, headY = ROWS / 2;
    int x_vel = 1, y_vel = 0;
    int appleX, appleY;
    int running = 1, newApple = 1, snakeLength = 0;
    Position body[COLS * ROWS];

    // Save original terminal settings
    struct termios original_settings;
    tcgetattr(STDIN_FILENO, &original_settings);
    setup_terminal(&original_settings);

    while (running)
    {
        if (newApple)
        {
            placeApple(&appleX, &appleY, body, snakeLength);
            newApple = 0;
        }

        init_aStar(headX, headY, appleX, appleY, &x_vel, &y_vel, body, snakeLength);
        NodeList bPath = run_aStar(headX, headY, appleX, appleY, &x_vel, &y_vel);
        for (size_t i = 0; i < bPath.size; i++)
        {
            printf("Step %zu: (%d,%d)\n", i, bPath.nodes[i].pos.x, bPath.nodes[i].pos.y);
        }

        render_game(appleX, appleY, body, snakeLength, headX, headY, bPath);

        // Limit frame rate
        usleep(5 * 1000000 / 60);

        adjustVelocity(&x_vel, &y_vel, headX, headY, appleX, appleY, bPath);
        updateGameState(&headX, &headY, x_vel, y_vel, &snakeLength, body, &appleX, &appleY, &newApple, &running);
    }
    if (!running)
    {
        printf("Game Over!\n");
    }

    // Restore original terminal settings before exiting
    restore_terminal(original_settings);
    return 0;
}

// find out if there is a cleaner way than 100000000 if else
void adjustVelocity(int *x_vel, int *y_vel, int headX, int headY, int appleX, int appleY, NodeList bPath)
{
    int nextX = bPath.nodes[bPath.size - 1].pos.x;
    int nextY = bPath.nodes[bPath.size - 1].pos.y;

    if (nextX == headX && nextY == headY - 1)
    {
        // Move up
        *x_vel = 0;
        *y_vel = -1;
    }
    else if (nextX == headX && nextY == headY + 1)
    {
        // Move down
        *x_vel = 0;
        *y_vel = 1;
    }
    else if (nextX == headX + 1 && nextY == headY)
    {
        // Move right
        *x_vel = 1;
        *y_vel = 0;
    }
    else if (nextX == headX - 1 && nextY == headY)
    {
        // Move left
        *x_vel = -1;
        *y_vel = 0;
    }
}
