/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:39:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 11:39:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"

int	terminal_putchar(int c)
{
	char	ch;

	ch = c;
	return (write(STDOUT_FILENO, &ch, 1));
}

int	terminal_init_caps(t_terminal *term)
{
	char	*term_env;

	if (!term)
		return (0);
	term->capabilities_loaded = 0;
	term->clear_to_eol = NULL;
	term->cursor_move = NULL;
	term_env = getenv("TERM");
	if (!term_env || tgetent(NULL, term_env) != 1)
		return (0);
	term->clear_to_eol = tgetstr("ce", NULL);
	term->cursor_move = tgetstr("cm", NULL);
	term->capabilities_loaded = 1;
	return (1);
}
