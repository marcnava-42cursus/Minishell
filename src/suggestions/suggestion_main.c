/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestion_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:42:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 11:42:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"

t_suggestion_ctx	*suggestion_init(const char *prompt)
{
	t_suggestion_ctx	*ctx;
	char				*path_env;

	ctx = malloc(sizeof(t_suggestion_ctx));
	if (!ctx)
		return (NULL);
	ctx->commands = malloc(sizeof(t_cmd_list));
	ctx->terminal = malloc(sizeof(t_terminal));
	if (!ctx->commands || !ctx->terminal)
	{
		free(ctx->commands);
		free(ctx->terminal);
		free(ctx);
		return (NULL);
	}
	cmdlist_init(ctx->commands);
	if (!terminal_init_caps(ctx->terminal))
	{
		free(ctx->commands);
		free(ctx->terminal);
		free(ctx);
		return (NULL);
	}
	ctx->prompt = sug_strdup(prompt);
	ctx->prompt_len = sug_strlen(prompt);
	path_env = getenv("PATH");
	cmdlist_load_from_path(ctx->commands, path_env);
	return (ctx);
}

static void	redraw_line(t_suggestion_ctx *ctx, const char *buffer, 
						size_t buffer_len)
{
	const char	*suggestion;

	if (ctx->terminal->cursor_move)
		tputs(tgoto(ctx->terminal->cursor_move, 0, 0), 1, terminal_putchar);
	else
		write(STDOUT_FILENO, "\r", 1);
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	write(STDOUT_FILENO, buffer, buffer_len);
	if (ctx->terminal->clear_to_eol)
		tputs(ctx->terminal->clear_to_eol, 1, terminal_putchar);
	suggestion = cmdlist_find_suggestion(ctx->commands, buffer);
	if (suggestion && buffer_len > 0)
	{
		write(STDOUT_FILENO, "\033[90m", 5);
		write(STDOUT_FILENO, suggestion + buffer_len, 
			sug_strlen(suggestion + buffer_len));
		write(STDOUT_FILENO, "\033[0m", 4);
	}
	if (ctx->terminal->cursor_move)
		tputs(tgoto(ctx->terminal->cursor_move, ctx->prompt_len + buffer_len, 0), 
			1, terminal_putchar);
}

char	*suggestion_readline(t_suggestion_ctx *ctx)
{
	char			buffer[SUG_BUFFER_SIZE];
	size_t			buffer_len;
	char			c;
	char			*result;
	struct termios	original_termios;

	if (!ctx)
		return (NULL);
	buffer_len = 0;
	ft_memset(buffer, 0, SUG_BUFFER_SIZE);
	enable_raw(&original_termios);
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) != 1)
			break ;
		if (c == 3)
		{
			disable_raw(&original_termios);
			write(STDOUT_FILENO, "\n", 1);
			exit(0);
		}
		else if (c == 127)
		{
			if (buffer_len > 0)
			{
				buffer_len--;
				buffer[buffer_len] = '\0';
			}
		}
		else if (c == '\r' || c == '\n')
		{
			disable_raw(&original_termios);
			if (ctx->terminal->clear_to_eol)
				tputs(ctx->terminal->clear_to_eol, 1, terminal_putchar);
			write(STDOUT_FILENO, "\n", 1);
			buffer[buffer_len] = '\0';
			result = sug_strdup(buffer);
			return (result);
		}
		else if (c >= 32 && c <= 126)
		{
			if (buffer_len + 1 < SUG_BUFFER_SIZE)
			{
				buffer[buffer_len++] = c;
				buffer[buffer_len] = '\0';
			}
		}
		redraw_line(ctx, buffer, buffer_len);
	}
	disable_raw(&original_termios);
	return (NULL);
}

void	suggestion_cleanup(t_suggestion_ctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->commands)
	{
		cmdlist_free(ctx->commands);
		free(ctx->commands);
	}
	if (ctx->terminal)
		free(ctx->terminal);
	if (ctx->prompt)
		free(ctx->prompt);
	free(ctx);
}
