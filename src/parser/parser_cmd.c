/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:22:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 20:40:26 by marcnava         ###   ########.fr       */
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

/* handle heredoc or input redirection */
static int	pc_token_is_redir_in(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx, char *tok)
{
	if (ft_strcmp(tok, "<<") == 0)
		return (pc_handle_heredoc(s, mshell, ctx));
	if (ft_strcmp(tok, "<") == 0)
		return (pc_handle_redir_in(s, mshell, ctx));
	return (0);
}

/* handle output redirection */
static int	pc_token_is_redir_out(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx, char *tok)
{
	int	append;

	append = (tok[1] == '>');
	return (pc_handle_redir_out(s, mshell, ctx, append));
}

/* handle normal word or glob */
static int	pc_token_is_word(t_pc_ctx *ctx, char *tok)
{
	return (pc_handle_word_or_glob(tok, ctx));
}

int	pc_apply_token(const char **s, t_mshell *mshell,
			t_pc_ctx *ctx, char *tok)
{
	int	ret;
	int	should_free;

	ret = 0;
	should_free = 1;
	if (ft_strcmp(tok, "<<") == 0 || ft_strcmp(tok, "<") == 0)
		ret = pc_token_is_redir_in(s, mshell, ctx, tok);
	else if (ft_strcmp(tok, ">") == 0 || ft_strcmp(tok, ">>") == 0)
		ret = pc_token_is_redir_out(s, mshell, ctx, tok);
	else
	{
		should_free = 0;
		ret = pc_token_is_word(ctx, tok);
	}
	if (ret < 0)
		ret = -1;
	if (should_free)
		ft_free((void **)&tok);
	return (ret);
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
			return (ft_free_matrix((void **)ctx->argv), -1);
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
		return (NULL);
	ctx.argv = ft_realloc_matrix(ctx.argv, ctx.argc, NULL);
	if (!ctx.argv)
		return (NULL);
	return (pc_finish(&ctx));
}
