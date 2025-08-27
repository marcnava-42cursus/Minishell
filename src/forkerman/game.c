/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:00:22 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 01:21:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

static void	process_input(t_game *g, char c)
{
	if (c == 'w')
		move_player(g, 0, -1);
	else if (c == 's')
		move_player(g, 0, 1);
	else if (c == 'a')
		move_player(g, -1, 0);
	else if (c == 'd')
		move_player(g, 1, 0);
	else if (c == ' ')
		place_bomb(g);
	else if (c == 'r')
		restart_game(g);
}

static void	render_frame(t_game *g)
{
	clear_scr();
	draw_game(g);
}

static int	check_victory(t_game *g)
{
	if (reached_goal(g))
	{
		printf("\n\nYou reached exit! Victory!\n");
		return (1);
	}
	return (0);
}

void	forkerman(void)
{
	t_game	g;
	char	c;

	restart_game(&g);
	enable_raw(&g.terminal_attrs);
    render_frame(&g);
	while (g.lives > 0)
	{
		if (read(0, &c, 1) != 1)
			continue ;
		if (c == 'q')
			break ;
		process_input(&g, c);
		update_bombs(&g);
		render_frame(&g);
		if (check_victory(&g))
			break ;
	}
	if (g.lives <= 0)
		printf("\n\nGame Over\n");
	disable_raw(&g.terminal_attrs);
}
