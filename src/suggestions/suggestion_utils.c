/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestion_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:11:39 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "suggestions.h"

static void	init_ctx_prompt(t_suggestion_ctx *ctx, const char *prompt)
{
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
}

static t_suggestion_ctx	*alloc_suggestion_ctx(void)
{
	t_suggestion_ctx	*ctx;

	ctx = malloc(sizeof(t_suggestion_ctx));
	if (!ctx)
		return (NULL);
	ctx->commands = NULL;
	ctx->terminal = NULL;
	ctx->prompt = NULL;
	ctx->commands = malloc(sizeof(t_cmd_list));
	if (!ctx->commands)
		return (ft_free((void **)&ctx), NULL);
	ctx->terminal = malloc(sizeof(t_terminal));
	if (!ctx->terminal)
		return (ft_free((void **)&ctx->commands), ft_free((void **)&ctx), NULL);
	return (ctx);
}

t_suggestion_ctx	*suggestion_init(const char *prompt)
{
	t_suggestion_ctx	*ctx;
	char				*path_env;

	ctx = alloc_suggestion_ctx();
	if (!ctx)
		return (NULL);
	cmdlist_init(ctx->commands);
	if (!terminal_init_caps(ctx->terminal))
		ctx->terminal->capabilities_loaded = 0;
	init_ctx_prompt(ctx, prompt);
	path_env = getenv("PATH");
	if (path_env)
		cmdlist_load_from_path(ctx->commands, path_env);
	return (ctx);
}

void	suggestion_cleanup(t_suggestion_ctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->commands)
	{
		cmdlist_free(ctx->commands);
		ft_free((void **)&ctx->commands);
	}
	if (ctx->terminal)
		ft_free((void **)&ctx->terminal);
	if (ctx->prompt)
		ft_free((void **)&ctx->prompt);
	ft_free((void **)&ctx);
}
