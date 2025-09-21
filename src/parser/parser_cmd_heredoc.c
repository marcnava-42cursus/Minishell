/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:07:20 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 01:07:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

int	pc_handle_heredoc(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*filename;
	int		fd;

	filename = get_next_token(s);
	if (!filename)
	{
		print_err2("minishell: syntax error near unexpected token `newline'\n",
			NULL, NULL);
		mshell->exit_code = 2;
		ft_free_matrix((void **)ctx->argv);
		return (-1);
	}
	fd = handle_heredoc(filename, mshell);
	ft_free((void **)&filename);
	if (fd == -1)
	{
		mshell->exit_code = 1;
		ctx->fd_in = -2;
		return (0);
	}
	ctx->fd_in = fd;
	return (0);
}
