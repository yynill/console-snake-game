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
void setup_terminal(struct termios *original_settings);
void restore_terminal(struct termios original_settings);
void placeApple(int *x, int *y, Position *body, int length);
void render_game(int appleX, int appleY, Position *body, int snakeLength, int headX, int headY);
void updateGameState(int *headX, int *headY, int x_vel, int y_vel, int *snakeLength, Position *body, int *appleX, int *appleY, int *newApple, int *running);

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

void setup_terminal(struct termios *original_settings)
{
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, original_settings);
    new_settings = *original_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void restore_terminal(struct termios original_settings)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
}

void placeApple(int *x, int *y, Position *body, int length)
{
    int isOccupied;

    do
    {
        isOccupied = 0;
        *x = rand() % COLS;
        *y = rand() % ROWS;

        // Check if the generated position is occupied by the snake's body
        for (int i = 0; i < length; i++)
        {
            if (body[i].x == *x && body[i].y == *y)
            {
                isOccupied = 1;
                break;
            }
        }
    } while (isOccupied);
}

void render_game(int appleX, int appleY, Position *body, int snakeLength, int headX, int headY)
{
    // Clear the screen to redraw the game state
    printf("\033[H\033[J"); // Clear the screen using ANSI escape codes

    // Print the current score
    printf("Score: %d\n", snakeLength);

    // Draw the top border
    printf("┌");
    for (int i = 0; i < COLS; i++)
    {
        printf("─");
    }
    printf("┐\n");

    // Draw the game field
    for (int j = 0; j < ROWS; j++)
    {
        printf("│");
        for (int i = 0; i < COLS; i++)
        {
            if (i == headX && j == headY)
            {
                printf("▓"); // Snake head
            }
            else
            {
                int isPrinted = 0;
                // Check for body segments
                for (int k = 0; k < snakeLength; k++)
                {
                    if (body[k].x == i && body[k].y == j)
                    {
                        printf("░"); // Snake body
                        isPrinted = 1;
                        break;
                    }
                }
                // Check for apple
                if (!isPrinted && i == appleX && j == appleY)
                {
                    printf("@"); // Apple
                }
                else if (!isPrinted)
                {
                    printf(" ");
                }
            }
        }
        printf("│\n");
    }

    // Draw the bottom border
    printf("└");
    for (int i = 0; i < COLS; i++)
    {
        printf("─");
    }
    printf("┘\n");
}

void updateGameState(int *headX, int *headY, int x_vel, int y_vel, int *snakeLength, Position *body, int *appleX, int *appleY, int *newApple, int *running)
{
    int prevX = *headX, prevY = *headY;
    // Update head position
    *headX += x_vel;
    *headY += y_vel;

    int tempX, tempY;

    // Update body position if snake length is greater than 0
    for (int k = *snakeLength - 1; k > 0; k--)
    {
        body[k] = body[k - 1];
    }
    if (*snakeLength > 0)
    {
        body[0].x = prevX;
        body[0].y = prevY;
    }

    // Check for body collisions
    for (int k = 1; k < *snakeLength; k++)
    {
        if (body[k].x == *headX && body[k].y == *headY)
        {
            *running = 0;
            break;
        }
    }

    // Check for border collisions
    if (*headX < 0 || *headX >= COLS || *headY < 0 || *headY >= ROWS)
    {
        *running = 0;
    }

    // Check for apple collisions
    if (*headX == *appleX && *headY == *appleY)
    {
        if (*snakeLength < COLS * ROWS - 1)
        {
            body[*snakeLength].x = *headX; // Grow snake at the new position
            body[*snakeLength].y = *headY;
            (*snakeLength)++;
        }
        else
        {
            printf("You've won the game!\n");
            *running = 0;
        }
        *newApple = 1;
    }
}
