#include "stdio.h"
#include "stdlib.h"

int slen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return i;
}
int min(int a, int b, int c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

void free_arr(void **ptr, int n)
{
	if (!ptr)
		return;
	for (int i = 0; i < n; i++)
		free(ptr[i]);
	free(ptr);
}

int bsq(FILE *file)
{
	char empty = 0, obstacle = 0, full = 0;
	int height = 0;
	if (fscanf(file, "%d %c %c %c\n", &height, &empty, &obstacle, &full) != 4)
		return 1;
	if (height <= 0 || empty == obstacle || empty == full || obstacle == full)
		return 1;
	char** map = calloc(height, sizeof(char*));
	int i = 0, w;
	char *line = NULL;
	size_t len = 0;
	for (; getline(&line, &len, file) != -1; i++)
	{
		if (i == height)
			return free(line), free_arr((void**)map, height), 1;
		map[i] = line;
		line = NULL;
		len = 0;
		if (i == 0)
		{
			w = slen(map[0]);
			if (w < 2 || map[0][w-1] != '\n')
				return free_arr((void**)map, height), 1;
		}
		else if (w != slen(map[i]) || map[i][w-1] != '\n')
			return free_arr((void**)map, height), 1;
	}
	if (i < height)
		return free(line), free_arr((void**)map, height), 1;
	int **dp = calloc(height + 1, sizeof(int*));
	int y = 0, x = 0, size = 0;
	for (int i = 0; i <= height; i++)
		dp[i] = calloc(w + 1, sizeof(int));
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < w - 1; j++)
		{
			if (map[i][j] == empty)
			{
				dp[i+1][j+1] = 1 + min(dp[i][j], dp[i+1][j], dp[i][j+1]);
				if (size < dp[i+1][j+1])
					y = i, x = j, size = dp[i+1][j+1]; 
			}
			else if (map[i][j] != obstacle)
				return free_arr((void**)map, height), free_arr((void**)dp, height + 1), 1;
		}
	}
	for (int i = y - size + 1; i <= y; i++)
	{
		for (int j = x - size + 1; j <= x; j++)
			map[i][j] = full;
	}
	for (int i = 0; i < height; i++)
		fprintf(stdout, "%s", map[i]);
	if (line)
		free(line);
	free_arr((void**)map, height);
	free_arr((void**)dp, height + 1);
	return 0;
}

int main(int ac, char **av)
{
	if (ac == 1)
	{
		if (bsq(stdin))
			fprintf(stderr, "fuck you\n");
	}
	else
	{
		for (int a = 1; a < ac; a++)
		{
			FILE *file = fopen(av[a], "r");
			if (!file || bsq(file))
				fprintf(stderr, "fuck you\n");
			if (file)
				fclose(file);
		}
	}
	return 0;
}