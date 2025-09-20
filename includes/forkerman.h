/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkerman.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:45:38 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 04:04:47 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORKERMAN_H
# define FORKERMAN_H

# include "utils.h"
# include "structs.h"

void	init_map(t_game *g);
void	restart_game(t_game *g);

void	draw_game(t_game *g);
void	place_bomb(t_game *g);
void	update_bombs(t_game *g);
void	move_player(t_game *g, int dx, int dy);
int		reached_goal(t_game *g);

void	forkerman(void);

#endif