#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#include "my_structs.h"

void setup_terminal(struct termios *original_settings);
void restore_terminal(struct termios original_settings);
void placeApple(int *x, int *y, Position *body, int length);
void render_game(int appleX, int appleY, Position *body, int snakeLength, int headX, int headY, NodeList bPath);
void updateGameState(int *headX, int *headY, int x_vel, int y_vel, int *snakeLength, Position *body, int *appleX, int *appleY, int *newApple, int *running);
void init_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel);
NodeList run_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel);

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
        init_aStar(headX, headY, appleX, appleY, &x_vel, &y_vel);

        if (newApple)
        {
            placeApple(&appleX, &appleY, body, snakeLength);
            newApple = 0;
        }

        updateGameState(&headX, &headY, x_vel, y_vel, &snakeLength, body, &appleX, &appleY, &newApple, &running);

        // Limit frame rate
        usleep(6 * 1000000 / 60);

        NodeList bPath = run_aStar(headX, headY, appleX, appleY, &x_vel, &y_vel);
        for (size_t i = 0; i < bPath.size; i++)
        {
            printf("Step %zu: (%d,%d)\n", i, bPath.nodes[i].pos.x, bPath.nodes[i].pos.y);
        }

        render_game(appleX, appleY, body, snakeLength, headX, headY, bPath);
    }
    if (!running)
    {
        printf("Game Over!\n");
    }

    // Restore original terminal settings before exiting
    restore_terminal(original_settings);
    return 0;
}