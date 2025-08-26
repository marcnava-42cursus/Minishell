/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:58:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 01:17:38 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

static char	get_tile_char(t_game *g, int x, int y)
{
	int	b;

	if (y == g->player_y && x == g->player_x)
		return ('P');
	b = 0;
	while (b < g->bomb_count)
	{
		if (g->bombs[b].x == x && g->bombs[b].y == y)
			return ('*');
		b++;
	}
	return (g->map[y][x]);
}

static void	draw_map(t_game *g)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			c = get_tile_char(g, x, y);
			write(1, &c, 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

void	draw_game(t_game *g)
{
	printf("\033[HVIDAS: %d  BOMBAS: %d\n",
		g->lives, g->bombs_available);
	draw_map(g);
}
