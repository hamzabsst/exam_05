#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int ac, char **av)
{
    if (ac != 4)
        return 1;

    int w = atoi(av[1]), h = atoi(av[2]), it = atoi(av[3]);
    int board[h][w];
    int next[h][w];

    // 1. Zero the board
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            board[y][x] = 0;

    // 2. Read commands and draw
    int x = 0, y = 0, draw = 0;
    char c;
    while (read(0, &c, 1) > 0)
    {
        if (c == 'x') draw = !draw;
        if (c == 'w' && y > 0) y--;
        if (c == 's' && y < h - 1) y++;
        if (c == 'a' && x > 0) x--;
        if (c == 'd' && x < w - 1) x++;
        if (draw) board[y][x] = 1;
    }

    // 3. Simulate Game of Life
    while (it--)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                // Count live neighbors around (x, y)
                int n = 0;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++)
                        if ((dx || dy) && y + dy >= 0 && y + dy < h && x + dx >= 0 && x + dx < w)
                            n += board[y + dy][x + dx];

                // Alive with 3 neighbors, OR (alive with 2 neighbors)
                next[y][x] = (n == 3 || (n == 2 && board[y][x]));
            }
        }
        // Copy next -> board
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
                board[y][x] = next[y][x];
    }

    // 4. Print board
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (board[y][x] == 1)
                putchar('O');
            else
                putchar(' ');
        }
        putchar('\n');
    }
    return 0;
}