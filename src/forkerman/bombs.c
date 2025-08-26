/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bombs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:58:48 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 01:15:25 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

static void	explode(t_game *g, t_bomb *b)
{
	static const int	dirs[5][2] = {{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	int					i;
	int					nx;
	int					ny;

	i = -1;
	while (++i < 5)
	{
		nx = b->x + dirs[i][0];
		ny = b->y + dirs[i][1];
		if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
		{
			if (g->map[ny][nx] == '+')
				g->map[ny][nx] = ' ';
			if (g->player_x == nx && g->player_y == ny)
				g->lives--;
		}
	}
}

void	update_bombs(t_game *g)
{
	int	i;

	i = 0;
	while (i < g->bomb_count)
	{
		if ((--g->bombs[i].timer) == 0)
		{
			explode(g, &g->bombs[i]);
			while (i < g->bomb_count - 1)
			{
				g->bombs[i] = g->bombs[i + 1];
				i++;
			}
			g->bomb_count--;
			g->bombs_available++;
			continue ;
		}
		i++;
	}
}

void	place_bomb(t_game *g)
{
	if (g->bombs_available <= 0 || g->bomb_count >= MAX_BOMBS)
		return ;
	g->bombs[g->bomb_count].x = g->player_x;
	g->bombs[g->bomb_count].y = g->player_y;
	g->bombs[g->bomb_count].timer = BOMB_TIMER;
	g->bomb_count++;
	g->bombs_available--;
}
