/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 17:56:45 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/29 17:56:47 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char *get_color_code(char *color)
{
	if (!ft_strcmp(color, "WHITE"))
		return ("\033[0;37m");
	if (!ft_strcmp(color, "GREEN"))
		return ("\033[0;32m");
	if (!ft_strcmp(color, "LIME"))
		return ("\033[1;32m");
	if (!ft_strcmp(color, "YELLOW"))
		return ("\033[0;33m");
	if (!ft_strcmp(color, "BLUE"))
		return ("\033[0;34m");
	if (!ft_strcmp(color, "RED"))
		return ("\033[0;31m");
	if (!ft_strcmp(color, "CYAN"))
		return ("\033[0;36m");
	if (!ft_strcmp(color, "MAGENTA"))
		return ("\033[0;35m");
	return ("");
}

char *reset_color(void)
{
	return ("\033[0m");
}
