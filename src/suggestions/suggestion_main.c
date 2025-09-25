/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestion_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:42:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/26 00:18:43 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"
#include "signals.h"
#include <errno.h>

volatile sig_atomic_t	g_in_suggestions = 0;

static void	display_suggestion(const char *suggestion, size_t buffer_len)
{
	size_t	suggestion_len;
	size_t	i;

	if (suggestion && buffer_len > 0)
	{
		write(STDOUT_FILENO, "\033[90m", 5);
		write(STDOUT_FILENO, suggestion + buffer_len,
			sug_strlen(suggestion + buffer_len));
		write(STDOUT_FILENO, "\033[0m", 4);
		suggestion_len = sug_strlen(suggestion + buffer_len);
		i = 0;
		while (i < suggestion_len)
		{
			write(STDOUT_FILENO, "\b", 1);
			i++;
		}
	}
}

void	redraw_line(t_suggestion_ctx *ctx, const char *buffer,
			size_t buffer_len)
{
	const char	*suggestion;

	write(STDOUT_FILENO, "\r", 1);
	if (ctx->terminal->clear_to_eol)
		tputs(ctx->terminal->clear_to_eol, 1, terminal_putchar);
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	write(STDOUT_FILENO, buffer, buffer_len);
	suggestion = cmdlist_find_suggestion(ctx->commands, buffer);
	display_suggestion(suggestion, buffer_len);
}

/*
** process_char returns:
**  0 -> keep reading
**  1 -> ENTER pressed (commit line)
**  2 -> CANCEL (Ctrl+C) requested while signals not delivered as SIGINT
**  3 -> EOF (Ctrl+D) on empty line
*/
static int	process_char(char c, t_readline_state *state)
{
	if (c == 3)
	{
		write(STDOUT_FILENO, "^C\n", 3);
		*state->buffer_len = 0;
		state->buffer[0] = '\0';
		return (g_signal_received = SIGINT, 2);
	}
	else if (c == 127 && *state->buffer_len > 0)
	{
		(*state->buffer_len)--;
		state->buffer[*state->buffer_len] = '\0';
	}
	else if (c == 4)
	{
		if (*state->buffer_len == 0)
			return (3);
	}
	else if (c == '\r' || c == '\n')
		return (1);
	else if (c >= 32 && c <= 126 && *state->buffer_len + 1 < SUG_BUFFER_SIZE)
	{
		state->buffer[(*state->buffer_len)++] = c;
		state->buffer[*state->buffer_len] = '\0';
	}
	return (0);
}

static char	*read_input_loop(t_suggestion_ctx *ctx, t_readline_state *state)
{
	ssize_t	r;
	char	c;
	int		action;
	char	*out;

	while (1)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == 1)
		{
			action = process_char(c, state);
			if (handle_action_code(ctx, state, action, &out))
				return (out);
		}
		else if (r < 0)
			return (handle_read_error(state));
		else
			break ;
	}
	return (NULL);
}

char	*suggestion_readline(t_suggestion_ctx *ctx)
{
	char				buffer[SUG_BUFFER_SIZE];
	size_t				buffer_len;
	struct termios		original_termios;
	t_readline_state	state;
	char				*result;

	if (!ctx)
		return (NULL);
	buffer_len = 0;
	ft_memset(buffer, 0, SUG_BUFFER_SIZE);
	state.buffer = buffer;
	state.buffer_len = &buffer_len;
	state.original_termios = &original_termios;
	g_in_suggestions = 1;
	enable_raw(&original_termios);
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	result = read_input_loop(ctx, &state);
	g_in_suggestions = 0;
	disable_raw(&original_termios);
	return (result);
}
