/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:59:36 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 12:59:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

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
