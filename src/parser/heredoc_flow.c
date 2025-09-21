/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_flow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:33:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:33:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	hd_write_line(int tmp_fd, char *line, t_mshell *mshell,
	int is_quoted)
{
	char	*expanded_line;

	if (is_quoted)
		return (write(tmp_fd, line, ft_strlen(line)), 0);
	expanded_line = expand_variables(line, mshell->envp, mshell->exit_code);
	if (expanded_line)
	{
		write(tmp_fd, expanded_line, ft_strlen(expanded_line));
		ft_free((void **)&expanded_line);
	}
	else
		write(tmp_fd, line, ft_strlen(line));
	return (0);
}

int	hd_loop_write(int tmp_fd, const char *clean_delim, t_mshell *mshell,
	int is_quoted)
{
	char	*line;

	printf("> ");
	line = readline("");
	while (line != NULL)
	{
		if (ft_strcmp(line, (char *)clean_delim) == 0)
		{
			ft_free((void **)&line);
			break ;
		}
		hd_write_line(tmp_fd, line, mshell, is_quoted);
		write(tmp_fd, "\n", 1);
		ft_free((void **)&line);
		printf("> ");
		line = readline("");
	}
	return (0);
}

int	hd_prepare_ctx(t_hd_ctx *c, t_mshell *mshell)
{
	const char	*pos;

	if (!mshell || !mshell->raw_command)
		return (-1);
	pos = hd_after_arrow(mshell->raw_command);
	c->read_fd = 0;
	c->original_delimiter = hd_extract_delim(pos);
	if (!c->original_delimiter)
		return (-1);
	c->is_quoted = check_heredoc_quotes(mshell->raw_command, NULL);
	if (c->is_quoted < 0)
		c->is_quoted = 0;
	else if (c->is_quoted > 0)
		c->is_quoted = 1;
	c->clean_delimiter = hd_clean_delimiter(c->original_delimiter,
			&c->is_quoted);
	ft_free((void **)&c->original_delimiter);
	if (!c->clean_delimiter)
		return (-1);
	return (0);
}
