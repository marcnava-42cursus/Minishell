/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:12:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:03:06 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

static int	is_operator_token(const char *tok)
{
	if (!tok)
		return (0);
	return (!ft_strcmp((char *)tok, "|")
		|| !ft_strcmp((char *)tok, "||")
		|| !ft_strcmp((char *)tok, "&")
		|| !ft_strcmp((char *)tok, "&&")
		|| !ft_strcmp((char *)tok, ">")
		|| !ft_strcmp((char *)tok, ">>")
		|| !ft_strcmp((char *)tok, "<")
		|| !ft_strcmp((char *)tok, "<<")
		|| !ft_strcmp((char *)tok, ")")
		|| !ft_strcmp((char *)tok, "("));
}

char	*pc_unquote(const char *str)
{
	char	*out;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			copy_squoted(str, &i, out, &j);
		else if (str[i] == '"')
			copy_dquoted(str, &i, out, &j);
		else
			out[j++] = str[i++];
	}
	out[j] = '\0';
	return (out);
}

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

	(void)ctx;

	filename = get_next_token(s);
	if (!filename)
	{
		print_err2(
			"minishell: syntax error near unexpected token `newline'\n",
			NULL, NULL);
		mshell->exit_code = 2;
		return (NULL);
	}
	if (is_operator_token(filename))
	{
		print_err2("minishell: syntax error near unexpected token `",
			filename, "'\n");
		mshell->exit_code = 2;
		ft_free((void **)&filename);
		return (NULL);
	}
	return (filename);
}

int	pc_init_ctx(t_pc_ctx *ctx)
{
	ctx->argv = ft_calloc(1, sizeof(char *));
	if (!ctx->argv)
		return (-1);
	ctx->argc = 0;
	ctx->fd_in = -1;
	ctx->fd_out = -1;
	return (0);
}
