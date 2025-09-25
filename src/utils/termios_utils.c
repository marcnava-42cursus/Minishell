/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:47:04 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/26 00:16:49 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	disable_raw(struct termios *terminal_attrs)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, terminal_attrs);
}

void	enable_raw(struct termios *terminal_attrs)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, terminal_attrs);
	raw = *terminal_attrs;
	raw.c_lflag &= ~(ICANON | ECHO | ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void	clear_scr(void)
{
	write(1, "\033[2J\033[H", 8);
}
