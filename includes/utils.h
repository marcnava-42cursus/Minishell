/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:07:39 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/05 19:10:12 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <termios.h>
# include <unistd.h>
# include "structs.h"

void	print_tree(t_ent *node, int indent);

void	enable_raw(struct termios *terminal_attrs);
void	disable_raw(struct termios *terminal_attrs);
void	clear_scr(void);

char	**ft_realloc_matrix(char **old, int oldc, char *new_str);

#endif