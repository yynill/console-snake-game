#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#include "my_structs.h"

void setup_terminal(struct termios *original_settings);
void restore_terminal(struct termios original_settings);
void placeApple(int *x, int *y, Position *body, int length);
void render_game(int appleX, int appleY, Position *body, int snakeLength, int headX, int headY);
void updateGameState(int *headX, int *headY, int x_vel, int y_vel, int *snakeLength, Position *body, int *appleX, int *appleY, int *newApple, int *running);

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
        render_game(appleX, appleY, body, snakeLength, headX, headY);

        if (newApple)
        {
            placeApple(&appleX, &appleY, body, snakeLength);
            newApple = 0;
        }

        updateGameState(&headX, &headY, x_vel, y_vel, &snakeLength, body, &appleX, &appleY, &newApple, &running);

        // Limit frame rate
        usleep(6 * 1000000 / 60);

        // Handle non-blocking input
        int ch;
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (FD_ISSET(STDIN_FILENO, &fds))
        {
            ch = getchar();
            if (ch == 'q')
                break; // Quit game
            if (ch == 'w' && y_vel != 1)
            {
                x_vel = 0;
                y_vel = -1;
            }
            else if (ch == 's' && y_vel != -1)
            {
                x_vel = 0;
                y_vel = 1;
            }
            else if (ch == 'a' && x_vel != 1)
            {
                x_vel = -1;
                y_vel = 0;
            }
            else if (ch == 'd' && x_vel != -1)
            {
                x_vel = 1;
                y_vel = 0;
            }
        }
    }
    if (!running)
    {
        printf("Game Over!\n");
    }

    // Restore original terminal settings before exiting
    restore_terminal(original_settings);
    return 0;
}