/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:47:04 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 12:50:23 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static struct termios  g_old;

void    disable_raw(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_old);
}

void    enable_raw(void)
{
	struct termios  raw;

	tcgetattr(STDIN_FILENO, &g_old);
	raw = g_old;
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void    clear_scr(void)
{
	write(1, "\033[2J\033[H", 8);
}
