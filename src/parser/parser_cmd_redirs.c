/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:07:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 01:07:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

int	pc_handle_redir_in(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*filename;
	int		fd;

	filename = pc_read_filename_or_error(s, mshell, ctx);
	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_err2("minishell: ", filename, ": ");
		print_err2(strerror(errno), "\n", NULL);
		mshell->exit_code = 1;
		ctx->fd_in = -2;
	}
	else
		pc_close_replace(&ctx->fd_in, fd);
	ft_free((void **)&filename);
	return (0);
}

int	pc_handle_redir_out(const char **s, t_mshell *mshell,
	t_pc_ctx *ctx, int append)
{
	char	*filename;
	int		fd;
	int		flags;

	filename = pc_read_filename_or_error(s, mshell, ctx);
	if (!filename)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		print_err2("minishell: ", filename, ": ");
		print_err2(strerror(errno), "\n", NULL);
		mshell->exit_code = 1;
		ctx->fd_out = -2;
	}
	else
		pc_close_replace(&ctx->fd_out, fd);
	ft_free((void **)&filename);
	return (0);
}
