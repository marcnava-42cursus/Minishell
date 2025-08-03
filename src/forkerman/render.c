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
