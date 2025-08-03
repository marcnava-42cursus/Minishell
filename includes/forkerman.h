/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkerman.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:45:38 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 13:30:40 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORKERMAN_H
# define FORKERMAN_H

# include "utils.h"

# define WIDTH		21
# define HEIGHT		11
# define MAX_BOMBS	99
# define BOMB_TIMER	5

typedef struct s_bomb
{
	int	x;
	int	y;
	int	timer;
}		t_bomb;

typedef struct s_game
{
	int		player_x;
	int		player_y;
	int		lives;
	int		bombs_available;
	int		bomb_count;
	t_bomb	bombs[MAX_BOMBS];
	char	map[HEIGHT][WIDTH + 1];
}			t_game;

void	init_map(t_game *g);
void	restart_game(t_game *g);

void	draw_game(t_game *g);
void	place_bomb(t_game *g);
void	update_bombs(t_game *g);
void	move_player(t_game *g, int dx, int dy);
int		reached_goal(t_game *g);

void	forkerman(void);

#endif