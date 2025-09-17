/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestion_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:42:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 19:09:10 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "suggestions.h"

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

static void	redraw_line(t_suggestion_ctx *ctx, const char *buffer,
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

static char	*handle_enter(t_suggestion_ctx *ctx, char *buffer,
				size_t buffer_len, struct termios *original_termios)
{
	char	*result;

	disable_raw(original_termios);
	write(STDOUT_FILENO, "\r", 1);
	if (ctx->terminal->clear_to_eol)
		tputs(ctx->terminal->clear_to_eol, 1, terminal_putchar);
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	write(STDOUT_FILENO, buffer, buffer_len);
	write(STDOUT_FILENO, "\n", 1);
	buffer[buffer_len] = '\0';
	result = sug_strdup(buffer);
	return (result);
}

static int	process_char(char c, t_readline_state *state)
{
	if (c == 3)
	{
		disable_raw(state->original_termios);
		write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}
	else if (c == 127 && *state->buffer_len > 0)
	{
		(*state->buffer_len)--;
		state->buffer[*state->buffer_len] = '\0';
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

char	*suggestion_readline(t_suggestion_ctx *ctx)
{
	char				buffer[SUG_BUFFER_SIZE];
	size_t				buffer_len;
	char				c;
	struct termios		original_termios;
	t_readline_state	state;

	if (!ctx)
		return (NULL);
	buffer_len = 0;
	ft_memset(buffer, 0, SUG_BUFFER_SIZE);
	state.buffer = buffer;
	state.buffer_len = &buffer_len;
	state.original_termios = &original_termios;
	enable_raw(&original_termios);
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) != 1)
			break ;
		if (process_char(c, &state))
			return (handle_enter(ctx, buffer, buffer_len, &original_termios));
		redraw_line(ctx, buffer, buffer_len);
	}
	disable_raw(&original_termios);
	return (NULL);
}
