/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_tok.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 21:02:13 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:02:24 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

static int	pc_token_is_redir_in(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx, char *tok)
{
	if (ft_strcmp(tok, "<<") == 0)
		return (pc_handle_heredoc(s, mshell, ctx));
	if (ft_strcmp(tok, "<") == 0)
		return (pc_handle_redir_in(s, mshell, ctx));
	return (0);
}

static int	pc_token_is_redir_out(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx, char *tok)
{
	int	append;

	append = (tok[1] == '>');
	return (pc_handle_redir_out(s, mshell, ctx, append));
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
		ret = pc_handle_word_or_glob(tok, ctx);
	}
	if (ret < 0)
		ret = -1;
	if (should_free)
		ft_free((void **)&tok);
	return (ret);
}