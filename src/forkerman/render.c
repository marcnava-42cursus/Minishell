/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:58:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 12:58:16 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

static void	draw_map(t_game *g)
{
	int	y;
	int	x;
	int	b;
	char	c;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			c = g->map[y][x];
			b = 0;
			if (y == g->player_y && x == g->player_x)
				c = 'P';
			else
			{
				while (b < g->bomb_count)
				{
					if (g->bombs[b].x == x && g->bombs[b].y == y)
					{
						c = '*';
						break ;
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

void	draw_game(t_game *g)
{
	printf("\033[HVIDAS: %d  BOMBAS: %d\n",
		g->lives, g->bombs_available);
	draw_map(g);
}
