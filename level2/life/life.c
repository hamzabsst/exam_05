#include "life.h"

int    count(int *g, int w, int h, int x, int y)
{
    int c = 0;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;
            int x1 = x + dx;
            int y1 = y + dy;
             if (x1 >= 0 && y1 >= 0 && x1 < w && y1 < h)
                c += g[y1 * w + x1];
        }
    return c;
}
 
int    main(int ac, char **av)
{
    if (ac != 4)
        return 1;
 
    int w = atoi(av[1]);
    int h = atoi(av[2]);
    int it = atoi(av[3]);
    int *g = calloc(w * h, sizeof(int));
    int *t = calloc(w * h, sizeof(int));
    int x = 0, y = 0, draw = 0;
    char c;

    while (read(0, &c, 1) > 0)
    {
        if (c == 'x')
            draw = !draw;
        if (c == 'w' && y > 0)
            y--;
        if (c == 's' && y < h - 1)
            y++;
        if (c == 'a' && x > 0)
            x--;
        if (c == 'd' && x < w - 1)
            x++;
        if (draw)
            g[y * w + x] = 1;   
    }
    while (it--)
    {
        for (int i = 0; i < w * h; i++)
        {
            int n = count(g, w, h, i % w, i / w);
            if (g[i] == 1)  
                t[i] = (n == 2 || n == 3);  
            else               
                t[i] = (n == 3);  
        }
        int *tmp = g; g = t; t = tmp;
    }
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
            putchar(g[y * w + x] ? 'O' : ' ');
        putchar('\n');
    }
    free(g);
    free(t);
}