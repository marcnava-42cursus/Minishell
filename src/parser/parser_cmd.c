/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:22:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:28:07 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

static int	pc_init_ctx(t_pc_ctx *ctx)
{
	ctx->argv = ft_calloc(1, sizeof(char *));
	if (!ctx->argv)
		return (-1);
	ctx->argc = 0;
	ctx->fd_in = -1;
	ctx->fd_out = -1;
	return (0);
}

static int	pc_control_or_end(const char **s)
{
	parser_skip_whitespace(s);
	if (!**s || **s == '|' || **s == '&' || **s == ')')
		return (1);
	return (0);
}

static int	pc_apply_token(const char **s, t_mshell *mshell,
							t_pc_ctx *ctx, char *tok)
{
	int	ret;
	int	append;
	int	should_free;

	ret = 0;
	append = 0;
	should_free = 1;
	if (ft_strcmp(tok, "<<") == 0)
	{
		if (pc_handle_heredoc(s, mshell, ctx) < 0)
			ret = -1;
	}
	else if (ft_strcmp(tok, "<") == 0)
	{
		if (pc_handle_redir_in(s, mshell, ctx) < 0)
			ret = -1;
	}
	else if (ft_strcmp(tok, ">") == 0 || ft_strcmp(tok, ">>") == 0)
	{
		append = (tok[1] == '>');
		if (pc_handle_redir_out(s, mshell, ctx, append) < 0)
			ret = -1;
	}
	else
	{
		/* Ownership of `tok` is consumed inside pc_handle_word_or_glob:
		   - Either it is added to ctx->argv (to be freed with the AST),
		   - Or it is freed internally when wildcard expansion occurs.
		*/
		should_free = 0;
		ret = pc_handle_word_or_glob(tok, ctx);
	}
	if (should_free)
		ft_free((void **)&tok);
	return (ret);
}

static int	pc_process_next(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*tok;

	if (pc_control_or_end(s))
		return (1);
	tok = get_next_token(s);
	if (!tok)
		return (1);
	return (pc_apply_token(s, mshell, ctx, tok));
}
/* Public: definida en parser.h */

t_ent	*parse_cmd(const char **s, t_mshell *mshell)
{
	t_pc_ctx	ctx;
	int			st;

	if (pc_init_ctx(&ctx) < 0)
		return (NULL);
	st = 0;
	while (st == 0)
		st = pc_process_next(s, mshell, &ctx);
	ctx.argv = ft_realloc_matrix(ctx.argv, ctx.argc, NULL);
	if (!ctx.argv)
		return (NULL);
	return (pc_finish(&ctx));
}
