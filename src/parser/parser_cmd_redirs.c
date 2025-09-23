/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:07:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 20:55:36 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

static int	pc_out_flags(int append)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}

int	pc_handle_redir_in(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*path;
	int		fd;

	if (pc_redir_blocked(ctx))
		return (pc_consume_filename(s, mshell, ctx));
	path = pc_read_path(s, mshell, ctx);
	if (!path)
		return (-1);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		pc_open_error(mshell, &ctx->fd_in, path);
	else
		pc_close_replace(&ctx->fd_in, fd);
	ft_free((void **)&path);
	return (0);
}

int	pc_handle_redir_out(const char **s, t_mshell *mshell,
			t_pc_ctx *ctx, int append)
{
	char	*path;
	int		fd;
	int		flags;

	if (pc_redir_blocked(ctx))
		return (pc_consume_filename(s, mshell, ctx));
	path = pc_read_path(s, mshell, ctx);
	if (!path)
		return (-1);
	flags = pc_out_flags(append);
	fd = open(path, flags, 0644);
	if (fd == -1)
		pc_open_error(mshell, &ctx->fd_out, path);
	else
		pc_close_replace(&ctx->fd_out, fd);
	ft_free((void **)&path);
	return (0);
}
