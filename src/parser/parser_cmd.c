/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:22:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:02:59 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

static int	pc_control_or_end(const char **s)
{
	parser_skip_whitespace(s);
	if (!**s || **s == '|' || **s == '&' || **s == ')')
		return (1);
	return (0);
}

static void	print_syntax_token(const char *tok)
{
	if (!tok || *tok == '\0')
		print_err2("minishell: syntax error near unexpected token `newline'\n",
			NULL, NULL);
	else
	{
		print_err2("minishell: syntax error near unexpected token `",
			tok, "'\n");
	}
}

static int	pc_process_next(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*tok;
	char	t[3];

	parser_skip_whitespace(s);
	if (pc_control_or_end(s))
	{
		if (ctx->argc == 0 && ctx->fd_in == -1 && ctx->fd_out == -1)
		{
			if (**s == '|' && *(*s + 1) == '|')
				print_syntax_token("||");
			else if (**s == '&' && *(*s + 1) == '&')
				print_syntax_token("&&");
			else if (**s == ')' || **s == '|')
				(ft_bzero(t, 3), t[0] = **s, print_syntax_token(t));
			else
				print_syntax_token(NULL);
			mshell->exit_code = 2;
			return (-1);
		}
		return (1);
	}
	tok = get_next_token(s);
	if (!tok)
		return (1);
	return (pc_apply_token(s, mshell, ctx, tok));
}

t_ent	*parse_cmd(const char **s, t_mshell *mshell)
{
	t_pc_ctx	ctx;
	int			st;

	if (pc_init_ctx(&ctx) < 0)
		return (NULL);
	st = 0;
	while (st == 0)
		st = pc_process_next(s, mshell, &ctx);
	if (st < 0)
	{
		ft_free_matrix((void **)ctx.argv);
		return (NULL);
	}
	ctx.argv = ft_realloc_matrix(ctx.argv, ctx.argc, NULL);
	if (!ctx.argv)
		return (NULL);
	return (pc_finish(&ctx));
}
