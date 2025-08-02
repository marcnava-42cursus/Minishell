/* === forkerman.h === */
#ifndef FORKERMAN_H
# define FORKERMAN_H

# include <termios.h>
# include <unistd.h>

# define WIDTH       21
# define HEIGHT      11
# define MAX_BOMBS   99
# define BOMB_TIMER  5

typedef struct s_bomb
{
	int x;
	int y;
	int timer;
}               t_bomb;

typedef struct s_game
{
	int     player_x;
	int     player_y;
	int     lives;
	int     bombs_available;
	int     bomb_count;
	t_bomb  bombs[MAX_BOMBS];
	char    map[HEIGHT][WIDTH + 1];
}               t_game;

void    enable_raw(void);
void    disable_raw(void);
void    clear_scr(void);

void    init_map(t_game *g);
void    restart_game(t_game *g);

void    draw_game(t_game *g);
void    place_bomb(t_game *g);
void    update_bombs(t_game *g);
void    move_player(t_game *g, int dx, int dy);
int     reached_goal(t_game *g);

void    bomberman(void);

#endif /* FORKERMAN_H */

/* === termios_utils.c === */
// #include "forkerman.h"

static struct termios  g_old;

void    disable_raw(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_old);
}

void    enable_raw(void)
{
	struct termios  raw;

	tcgetattr(STDIN_FILENO, &g_old);
	raw = g_old;
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void    clear_scr(void)
{
	write(1, "\033[2J\033[H", 8);
}

/* === map.c === */
// #include "forkerman.h"

void    init_map(t_game *g)
{
	static const char layout[HEIGHT][WIDTH + 1] = {
		"#####################",
		"#  +# + +#   # + B +#",
		"# # ### ### ### ### #",
		"# # +   + #   + #   #",
		"# ### # ### # ### # #",
		"# + B #   + #   B + #",
		"# ### ##### ### ### #",
		"#     + B #   +   + #",
		"# ### ### ### ### # #",
		"#   +   + B +   +  S#",
		"#####################"
	};
	int i;

	i = 0;
	while (i < HEIGHT)
	{
		int j = 0;

		while (j < WIDTH)
		{
			g->map[i][j] = layout[i][j];
			j++;
		}
		g->map[i][WIDTH] = '\0';
		i++;
	}
	g->player_x = 1;
	g->player_y = 1;
}

void    restart_game(t_game *g)
{
	init_map(g);
	g->lives = 3;
	g->bombs_available = 2;
	g->bomb_count = 0;
}

/* === render.c === */
// #include "forkerman.h"
#include <stdio.h>

void    draw_game(t_game *g)
{
	int y;

	printf("\033[HVIDAS: %d  BOMBAS: %d\n",
		   g->lives, g->bombs_available);
	y = 0;
	while (y < HEIGHT)
	{
		int x = 0;

		while (x < WIDTH)
		{
			char c = g->map[y][x];
			int  b = 0;

			if (y == g->player_y && x == g->player_x)
				c = 'P';
			else
			{
				while (b < g->bomb_count)
				{
					if (g->bombs[b].x == x && g->bombs[b].y == y)
					{
						c = '*';
						break;
					}
					b++;
				}
			}
			write(1, &c, 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

/* === bombs.c === */
// #include "forkerman.h"

static void explode(t_game *g, t_bomb *b)
{
	int dirs[5][2] = {{0,0},{1,0},{-1,0},{0,1},{0,-1}};
	int i = 0;

	while (i < 5)
	{
		int nx = b->x + dirs[i][0];
		int ny = b->y + dirs[i][1];

		if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
		{
			if (g->map[ny][nx] == '+')
				g->map[ny][nx] = ' ';
			if (g->player_x == nx && g->player_y == ny)
				g->lives--;
		}
		i++;
	}
}

void    update_bombs(t_game *g)
{
	int i = 0;

	while (i < g->bomb_count)
	{
		if (--g->bombs[i].timer == 0)
		{
			explode(g, &g->bombs[i]);
			while (i < g->bomb_count - 1)
			{
				g->bombs[i] = g->bombs[i + 1];
				i++;
			}
			g->bomb_count--;
			g->bombs_available++;
			continue;
		}
		i++;
	}
}

void    place_bomb(t_game *g)
{
	if (g->bombs_available <= 0 || g->bomb_count >= MAX_BOMBS)
		return;
	g->bombs[g->bomb_count].x = g->player_x;
	g->bombs[g->bomb_count].y = g->player_y;
	g->bombs[g->bomb_count].timer = BOMB_TIMER;
	g->bomb_count++;
	g->bombs_available--;
}

/* === player.c === */
// #include "forkerman.h"

void    move_player(t_game *g, int dx, int dy)
{
	int nx = g->player_x + dx;
	int ny = g->player_y + dy;

	if (nx > 0 && nx < WIDTH && ny > 0 && ny < HEIGHT
		&& g->map[ny][nx] != '#' && g->map[ny][nx] != '+')
	{
		g->player_x = nx;
		g->player_y = ny;
		if (g->map[ny][nx] == 'B' && g->bombs_available < MAX_BOMBS)
		{
			g->bombs_available++;
			g->map[ny][nx] = ' ';
		}
	}
}

int     reached_goal(t_game *g)
{
	return (g->map[g->player_y][g->player_x] == 'S');
}

/* === game.c === */
// #include "forkerman.h"
#include <stdio.h>

void    bomberman(void)
{
	t_game  g;
	char    c;

	restart_game(&g);
	enable_raw();
	clear_scr();
	draw_game(&g);
	while (g.lives > 0)
	{
		if (read(0, &c, 1) != 1)
			continue;
		if (c == 'q')
			break;
		if (c == 'w')
			move_player(&g, 0, -1);
		else if (c == 's')
			move_player(&g, 0, 1);
		else if (c == 'a')
			move_player(&g, -1, 0);
		else if (c == 'd')
			move_player(&g, 1, 0);
		else if (c == ' ')
			place_bomb(&g);
		else if (c == 'r')
			restart_game(&g);
		update_bombs(&g);
		clear_scr();
		draw_game(&g);
		if (reached_goal(&g))
		{
			printf("\n\nYou reached exit! Victory!\n");
			break;
		}
	}
	if (g.lives <= 0)
		printf("\n\nGame Over\n");
	disable_raw();
}

/* === main.c === */
// #include "forkerman.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("ASCII FORKERMAN - Puzzle with exit\n");
	printf("WASD=move SPACE=bomb R=restart Q=quit\n");
	printf("Press any key to start...\n");
	read(0, &(char){0}, 1);
	bomberman();
	exit(0);
}
