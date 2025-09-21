/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_words.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:07:40 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 01:07:40 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "wildcards.h"
#include "utils.h"

int	pc_handle_word_or_glob(char *tok, t_pc_ctx *ctx)
{
	int		mcount;
	char	**matches;
	int		i;

	if (wc_token_should_expand(tok))
	{
		mcount = 0;
		matches = wc_expand(tok, &mcount);
		if (matches && mcount > 0)
		{
			i = 0;
			while (i < mcount)
			{
				ctx->argv = ft_realloc_matrix(ctx->argv, ctx->argc, matches[i]);
				ctx->argc++;
				i++;
			}
			ft_free((void **)&tok);
			ft_free((void **)&matches);
			return (0);
		}
	}
	ctx->argv = ft_realloc_matrix(ctx->argv, ctx->argc, tok);
	ctx->argc++;
	return (0);
}

t_ent	*pc_finish(const t_pc_ctx *ctx)
{
	t_ent	*node;

	node = ent_new_node(NODE_COMMAND, ctx->argv);
	if (node)
	{
		node->fd_in = ctx->fd_in;
		node->fd_out = ctx->fd_out;
	}
	return (node);
}
