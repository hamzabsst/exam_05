#include "life.h"

int main(int ac, char **av)
{
    if(ac != 4)
        return 1;
    int w = atoi(av[1]);
    int h = atoi(av[2]);
    int it = atoi(av[3]);

    int board[h][w];
    int next[h][w];

    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
            board[y][x] = 0;
    
    int x = 0, y = 0, draw = 0;
    char c;
    while (read(0, &c, 1) > 0)
    {
        if(c == 'x') draw = !draw;
        if(c == 'w' && y > 0) y--;
        if(c == 's' && y + 1 < h) y++;
        if(c == 'a' && x > 0) x--;
        if(c == 'd' && x + 1 < w) x++;
        if(draw) board[y][x] = 1;
    }
    while (it--)
    {
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                int count = 0;
                for(int dy = -1; dy <= 1; dy++)
                {
                    for(int dx = -1; dx <= 1; dx++)
                    {
                        if((dy || dx) && y + dy >= 0 && dy + y < h && dx + x >= 0 && dx + x < w)
                        {
                            count += board[y + dy][x + dx];
                        }
                    }
                }
                next[y][x] = (count == 3 || (count == 2 && board[y][x]));
            }
        }
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                board[y][x] = next[y][x];
            }
        }
    }
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            if(board[y][x] == 1)
                putchar('O');
            else
                putchar(' ');
        }
        putchar('\n');
    }
    return 0;
}