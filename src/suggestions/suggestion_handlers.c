/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestion_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 21:38:58 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 21:39:44 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"
#include "signals.h"
#include <errno.h>

char	*handle_enter(t_suggestion_ctx *ctx, char *buffer,
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

int	handle_action_code(t_suggestion_ctx *ctx, t_readline_state *state,
				int action, char **out)
{
	if (action == 1)
	{
		*out = handle_enter(ctx, state->buffer, *state->buffer_len,
				state->original_termios);
		return (1);
	}
	else if (action == 2)
	{
		disable_raw(state->original_termios);
		*out = sug_strdup("");
		return (1);
	}
	else if (action == 3)
	{
		write(STDOUT_FILENO, "\n", 1);
		disable_raw(state->original_termios);
		*out = NULL;
		return (1);
	}
	redraw_line(ctx, state->buffer, *state->buffer_len);
	return (0);
}

char	*handle_read_error(t_readline_state *state)
{
	if (errno == EINTR)
	{
		disable_raw(state->original_termios);
		return (sug_strdup(""));
	}
	return (NULL);
}
