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
	char	*path;
	int		fd;

	/* Si ya estamos en estado de error en cualquier redirección, solo consumir el token */
	if (ctx->fd_in == -2 || ctx->fd_out == -2)
	{
		filename = pc_read_filename_or_error(s, mshell, ctx);
		if (filename)
			ft_free((void **)&filename);
		return (0);
	}
	filename = pc_read_filename_or_error(s, mshell, ctx);
	if (!filename)
		return (-1);
	path = pc_unquote(filename);
	if (!path)
		return (ft_free((void **)&filename), -1);
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		print_err2("minishell: ", path, ": ");
		print_err2(strerror(errno), "\n", NULL);
		mshell->exit_code = 1;
		ctx->fd_in = -2;
	}
	else
		pc_close_replace(&ctx->fd_in, fd);
	ft_free((void **)&filename);
	ft_free((void **)&path);
	return (0);
}

int	pc_handle_redir_out(const char **s, t_mshell *mshell,
	t_pc_ctx *ctx, int append)
{
	char	*filename;
	char	*path;
	int		fd;
	int		flags;

	/* Si ya estamos en estado de error en cualquier redirección, solo consumir el token */
	if (ctx->fd_out == -2 || ctx->fd_in == -2)
	{
		filename = pc_read_filename_or_error(s, mshell, ctx);
		if (filename)
			ft_free((void **)&filename);
		return (0);
	}
	filename = pc_read_filename_or_error(s, mshell, ctx);
	if (!filename)
		return (-1);
	path = pc_unquote(filename);
	if (!path)
		return (ft_free((void **)&filename), -1);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd == -1)
	{
		print_err2("minishell: ", path, ": ");
		print_err2(strerror(errno), "\n", NULL);
		mshell->exit_code = 1;
		ctx->fd_out = -2;
	}
	else
		pc_close_replace(&ctx->fd_out, fd);
	ft_free((void **)&filename);
	ft_free((void **)&path);
	return (0);
}
