/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:12:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:12:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

void	pc_close_replace(int *dst_fd, int new_fd)
{
	if (*dst_fd != -1 && *dst_fd != -2)
		close(*dst_fd);
	*dst_fd = new_fd;
}

char	*pc_read_filename_or_error(const char **s, t_mshell *mshell,
								t_pc_ctx *ctx)
{
	char	*filename;

	filename = get_next_token(s);
	if (!filename)
	{
		print_err2(
			"minishell: syntax error near unexpected token `newline'\n",
			NULL, NULL);
		mshell->exit_code = 2;
		ft_free_matrix((void **)ctx->argv);
		return (NULL);
	}
	return (filename);
}
