#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#define COLS 60
#define ROWS 30

typedef struct
{
    int x;
    int y;
} Position;

// Function prototypes...
void setup_terminal();
void restore_terminal(struct termios original_settings);
void placeApple(int *x, int *y);

// Function to setup terminal attributes for non-canonical mode
void setup_terminal()
{
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

// Function to restore original terminal settings
void restore_terminal(struct termios original_settings)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
}

void placeApple(int *x, int *y)
{
    int min = 1;

    *x = min + rand() % (COLS - min + 1);
    *y = min + rand() % (ROWS - min + 1);
}

int main()
{
    // Save original terminal settings
    struct termios original_settings;
    tcgetattr(STDIN_FILENO, &original_settings);

    // Set up terminal for non-blocking input
    setup_terminal();

    // Game variables
    int x = COLS / 2, y = ROWS / 2;
    int x_vel = 1, y_vel = 0;
    int appleX, appleY;
    int running = 1, newApple = 1, snakeLength = 1;
    Position body[COLS * ROWS];

    while (running)
    {
        system("clear");

        // Render game area
        printf("┌");
        for (int i = 0; i < COLS; i++)
            printf("─");
        printf("┐\n");

        for (int j = 0; j < ROWS; j++)
        {
            printf("│");
            for (int i = 0; i < COLS; i++)
            {
                for (int k = 0; k < snakeLength; k++)
                {
                    if (body[k].x == i && body[k].y == j)
                    {
                        printf("░"); // Print the snake body part
                        break;
                    }
                }
                if (j == y && i == x)
                {
                    printf("▓");
                }
                else if (j == appleY && i == appleX)
                {
                    printf("@");
                }
                else
                    printf("·");
            }
            printf("│\n");
        }
        printf("└");
        for (int i = 0; i < COLS; i++)
            printf("─");
        printf("┘\n");

        // Update apple position
        if (newApple)
        {
            placeApple(&appleX, &appleY);
            newApple = 0;
        }

        // Update head position
        x += x_vel;
        y += y_vel;

        // border collisions
        if (x < 0 || x >= COLS || y < 0 || y >= ROWS)
        {
            running = 0; // Set running to 0 to stop the game
        }

        // apple collisions
        if (x == appleX && y == appleY)
        {
            if (snakeLength < COLS * ROWS)
            {
                body[snakeLength - 1].x = x;
                body[snakeLength - 1].y = y;
                snakeLength++;
            }
            else
            {
                printf("You've won the game!");
                running = 0;
            }
            newApple = 1;
        }

        // Limit frame rate
        usleep(7 * 1000000 / 60);

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

        // check segments
        printf("Current snake body:\n");
        for (int i = 0; i < snakeLength; i++)
        {
            printf("Segment %d: (%d, %d)\n", i, body[i].x, body[i].y);
        }
    }

    // Restore original terminal settings before exiting
    restore_terminal(original_settings);
    return 0;
}