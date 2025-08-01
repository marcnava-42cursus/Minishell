#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 21
#define HEIGHT 11
#define MAX_BOMBS 99
#define BOMB_TIMER 5

typedef struct s_bomb {
	int x, y;
	int timer;
}	t_bomb;

typedef struct s_game {
	int player_x;
	int player_y;
	int lives;
	int bombs_available;
	int bomb_count;
	t_bomb bombs[MAX_BOMBS];
	char map[HEIGHT][WIDTH + 1];
}	t_game;

static struct termios old_termios;

void disable_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

void enable_raw_mode(void)
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, &old_termios);
	atexit(disable_raw_mode);
	raw = old_termios;
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void clear_screen(void)
{
	write(1, "\033[2J\033[H", 8);
}

void init_map(t_game *g)
{
	const char *layout[HEIGHT] = {
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
	for (int i = 0; i < HEIGHT; i++)
	{
		strncpy(g->map[i], layout[i], WIDTH);
		g->map[i][WIDTH] = '\0';
	}
	g->player_x = 1;
	g->player_y = 1;
}

void draw_game(t_game *g)
{
	char buf[2048];
	int len = 0;

	len += snprintf(buf + len, sizeof(buf) - len,
		"\033[HVIDAS: %d  BOMBAS: %d\n", g->lives, g->bombs_available);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (y == g->player_y && x == g->player_x)
				len += snprintf(buf + len, sizeof(buf) - len, "P");
			else {
				int is_bomb = 0;
				for (int b = 0; b < g->bomb_count; b++)
					if (g->bombs[b].x == x && g->bombs[b].y == y)
						is_bomb = 1;
				if (is_bomb)
					len += snprintf(buf + len, sizeof(buf) - len, "*");
				else
					len += snprintf(buf + len, sizeof(buf) - len, "%c", g->map[y][x]);
			}
		}
		len += snprintf(buf + len, sizeof(buf) - len, "\n");
	}
	write(1, buf, len);
}

void place_bomb(t_game *g)
{
	if (g->bombs_available <= 0 || g->bomb_count >= MAX_BOMBS)
		return;
	t_bomb *b = &g->bombs[g->bomb_count++];
	b->x = g->player_x;
	b->y = g->player_y;
	b->timer = BOMB_TIMER;
	g->bombs_available--;
}

void explode(t_game *g, t_bomb *b)
{
	int dx[5] = {0, 1, -1, 0, 0};
	int dy[5] = {0, 0, 0, 1, -1};
	for (int i = 0; i < 5; i++)
	{
		int x = b->x + dx[i];
		int y = b->y + dy[i];
		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
			continue;
		if (g->map[y][x] == '+')
			g->map[y][x] = ' ';
		if (g->player_x == x && g->player_y == y)
			g->lives--;
	}
}

void update_bombs(t_game *g)
{
	for (int i = 0; i < g->bomb_count; i++)
	{
		if (--g->bombs[i].timer == 0)
		{
			explode(g, &g->bombs[i]);
			for (int j = i; j < g->bomb_count - 1; j++)
				g->bombs[j] = g->bombs[j + 1];
			g->bomb_count--;
			i--;
		}
	}
}

void pick_bonus(t_game *g)
{
	if (g->map[g->player_y][g->player_x] == 'B')
	{
		if (g->bombs_available < MAX_BOMBS)
			g->bombs_available++;
		g->map[g->player_y][g->player_x] = ' ';
	}
}

int reached_goal(t_game *g)
{
	return (g->map[g->player_y][g->player_x] == 'S');
}

void move_player(t_game *g, int dx, int dy)
{
	int x = g->player_x + dx;
	int y = g->player_y + dy;
	if (x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		if (g->map[y][x] != '#' && g->map[y][x] != '+')
		{
			g->player_x = x;
			g->player_y = y;
			pick_bonus(g);
		}
}

void restart_level(t_game *g)
{
	init_map(g);
	g->lives = 3;
	g->bombs_available = 2;
	g->bomb_count = 0;
}

void bomberman(void)
{
	t_game g;
	restart_level(&g);
	char c;
	enable_raw_mode();
	clear_screen();
	draw_game(&g);
	while (g.lives > 0)
	{
		if (read(0, &c, 1) == 1)
		{
			if (c == 'q') break;
			else if (c == 'w') move_player(&g, 0, -1);
			else if (c == 's') move_player(&g, 0, 1);
			else if (c == 'a') move_player(&g, -1, 0);
			else if (c == 'd') move_player(&g, 1, 0);
			else if (c == ' ') place_bomb(&g);
			else if (c == 'r') restart_level(&g);
			update_bombs(&g);
			clear_screen();
			draw_game(&g);
			if (reached_goal(&g))
			{
				write(1, "\n\n¡Has llegado a la salida! ¡Victoria!\n", 40);
				break ;
			}
		}
	}
	if (g.lives <= 0)
		write(1, "\n\nGame Over\n", 13);
	disable_raw_mode();
}

int main(void)
{
	write(1, "ASCII BOMBERMAN - Puzzle con salida\n", 36);
	write(1, "WASD = mover | ESPACIO = bomba | R = reiniciar | Q = salir\n", 58);
	write(1, "Presiona una tecla para comenzar...\n", 37);
	getchar();
	bomberman();
	return (0);
}
