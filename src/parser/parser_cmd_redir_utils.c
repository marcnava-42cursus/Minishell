/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redir_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 20:54:09 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 20:54:52 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

/* return 1 if any redir already failed in this command context */
int	pc_redir_blocked(t_pc_ctx *ctx)
{
	return (ctx->fd_in == -2 || ctx->fd_out == -2);
}

/* consume next filename token (for error-shortcircuit paths) */
int	pc_consume_filename(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*filename;

	filename = pc_read_filename_or_error(s, mshell, ctx);
	if (filename)
		ft_free((void **)&filename);
	return (0);
}

/* read filename and return unquoted path (malloc'ed). NULL on error */
char	*pc_read_path(const char **s, t_mshell *mshell, t_pc_ctx *ctx)
{
	char	*filename;
	char	*path;

	filename = pc_read_filename_or_error(s, mshell, ctx);
	if (!filename)
		return (NULL);
	path = pc_unquote(filename);
	ft_free((void **)&filename);
	return (path);
}

/* common open() error path for in/out redirs */
void	pc_open_error(t_mshell *mshell, int *ctx_fd_target, const char *path)
{
	print_err2("minishell: ", path, ": ");
	print_err2(strerror(errno), "\n", NULL);
	mshell->exit_code = 1;
	*ctx_fd_target = -2;
}
