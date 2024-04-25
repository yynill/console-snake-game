# Terminal Snake Game

## Overview

A simple terminal-based version of the classic Snake game, implemented in C.

![Screen Shot 2024-04-20 at 17 26 45 PM](https://github.com/yynill/console-snake-game/assets/145238134/0d4f70ff-b25e-4c11-a085-bb8dbd96d8a8)

## Prerequisites

Before running the game, make sure you have a C compiler like `gcc` installed on your system. This game uses standard libraries such as `stdlib.h`, `stdio.h`, `unistd.h`, `termios.h`, and `sys/select.h`.

## Compilation

Compile the game using the following command:

```bash
gcc manual.c snake.c -o snake
```

To run the game, use the following command:

```bash
./snake
```

## Controls

W: Move up
S: Move down
A: Move left
D: Move right
Q: Quit the game

## Bot

For those interested in automating gameplay, you can compile the game with a bot feature. Use this command to compile both the game and the bot:

Compile the game and bot using the following command:

```bash
gcc bot.c snake.c a_star.c -o snakeBot
```

To run the bot, enter:

```bash
./snakeBot
```
