/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:55:09 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 01:11:23 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

static void	copy_layout_row(t_game *g,
	const char layout[HEIGHT][WIDTH + 1], int i)
{
	int	j;

	j = 0;
	while (j < WIDTH)
	{
		g->map[i][j] = layout[i][j];
		j++;
	}
	g->map[i][WIDTH] = '\0';
}

void	init_map(t_game *g)
{
	static const char	layout[HEIGHT][WIDTH + 1] = {
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
	int					i;

	i = 0;
	while (i < HEIGHT)
	{
		copy_layout_row(g, layout, i);
		i++;
	}
	g->player_x = 1;
	g->player_y = 1;
}

void	restart_game(t_game *g)

{
	init_map(g);
	g->lives = 3;
	g->bombs_available = 2;
	g->bomb_count = 0;
}
