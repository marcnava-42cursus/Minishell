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
	
	/* Initialize all pointers to NULL for safe cleanup */
	ctx->commands = NULL;
	ctx->terminal = NULL;
	ctx->prompt = NULL;
	
	/* Allocate commands list */
	ctx->commands = malloc(sizeof(t_cmd_list));
	if (!ctx->commands)
	{
		free(ctx);
		return (NULL);
	}
	
	/* Allocate terminal */
	ctx->terminal = malloc(sizeof(t_terminal));
	if (!ctx->terminal)
	{
		free(ctx->commands);
		free(ctx);
		return (NULL);
	}
	
	/* Initialize command list */
	cmdlist_init(ctx->commands);
	
	/* Initialize terminal capabilities */
	if (!terminal_init_caps(ctx->terminal))
	{
		/* Continue without terminal capabilities - fallback mode */
		ctx->terminal->capabilities_loaded = 0;
	}
	
	/* Set up prompt */
	if (prompt && sug_strlen(prompt) > 0)
	{
		ctx->prompt = sug_strdup(prompt);
		if (!ctx->prompt)
		{
			ctx->prompt = sug_strdup("> ");
			ctx->prompt_len = 2;
		}
		else
			ctx->prompt_len = sug_strlen(ctx->prompt);
	}
	else
	{
		ctx->prompt = sug_strdup("> ");
		ctx->prompt_len = 2;
	}
	
	/* Load commands from PATH */
	path_env = getenv("PATH");
	if (path_env)
		cmdlist_load_from_path(ctx->commands, path_env);
	
	return (ctx);
}


static void	redraw_line(t_suggestion_ctx *ctx, const char *buffer, 
						size_t buffer_len)
{
	const char	*suggestion;

	/* Move to beginning of current line */
	write(STDOUT_FILENO, "\r", 1);
	
	/* Clear entire line */
	if (ctx->terminal->clear_to_eol)
		tputs(ctx->terminal->clear_to_eol, 1, terminal_putchar);
	
	/* Write prompt */
	write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
	
	/* Write user input */
	write(STDOUT_FILENO, buffer, buffer_len);
	
	/* Add suggestion in gray if there is one */
	suggestion = cmdlist_find_suggestion(ctx->commands, buffer);
	if (suggestion && buffer_len > 0)
	{
		write(STDOUT_FILENO, "\033[90m", 5);
		write(STDOUT_FILENO, suggestion + buffer_len, 
			sug_strlen(suggestion + buffer_len));
		write(STDOUT_FILENO, "\033[0m", 4);
	}
	
	/* Position cursor after user input using backspace */
	
	/* The cursor is already in the right place after writing the content */
	/* Just move it back from the suggestion to after the user input */
	if (suggestion && buffer_len > 0)
	{
		/* We need to move cursor back from end of suggestion to end of user input */
		size_t suggestion_len = sug_strlen(suggestion + buffer_len);
		size_t i;
		
		/* Move cursor backward by suggestion length using backspace */
		for (i = 0; i < suggestion_len; i++)
			write(STDOUT_FILENO, "\b", 1);
	}
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
			/* Clear line completely before finishing */
			write(STDOUT_FILENO, "\r", 1);
			if (ctx->terminal->clear_to_eol)
				tputs(ctx->terminal->clear_to_eol, 1, terminal_putchar);
			/* Rewrite prompt and command without suggestions */
			write(STDOUT_FILENO, ctx->prompt, ctx->prompt_len);
			write(STDOUT_FILENO, buffer, buffer_len);
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

void	suggestion_update_prompt(t_suggestion_ctx *ctx, const char *new_prompt)
{
	if (!ctx)
		return ;
	if (ctx->prompt)
		free(ctx->prompt);
	if (new_prompt && sug_strlen(new_prompt) > 0)
	{
		ctx->prompt = sug_strdup(new_prompt);
		if (ctx->prompt)
			ctx->prompt_len = sug_strlen(ctx->prompt);
		else
		{
			ctx->prompt = sug_strdup("> ");
			ctx->prompt_len = 2;
		}
	}
	else
	{
		ctx->prompt = sug_strdup("> ");
		ctx->prompt_len = 2;
	}
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
