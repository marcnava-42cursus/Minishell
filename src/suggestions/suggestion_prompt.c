/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestion_prompt.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:11:39 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"

static void	set_default_prompt(t_suggestion_ctx *ctx)
{
	ctx->prompt = sug_strdup("> ");
	ctx->prompt_len = 2;
}

void	suggestion_update_prompt(t_suggestion_ctx *ctx, const char *new_prompt)
{
	if (!ctx)
		return ;
	if (ctx->prompt)
		ft_free((void **)&ctx->prompt);
	if (new_prompt && sug_strlen(new_prompt) > 0)
	{
		ctx->prompt = sug_strdup(new_prompt);
		if (ctx->prompt)
			ctx->prompt_len = sug_strlen(ctx->prompt);
		else
			set_default_prompt(ctx);
	}
	else
		set_default_prompt(ctx);
}
