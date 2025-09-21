/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:23:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:28:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	hd_make_tmpfile(t_hd_ctx *c)
{
	c->count_str = ft_itoa(c->read_fd);
	if (!c->count_str)
		return (-1);
	c->tmp_filename = ft_strjoin("/tmp/heredoc_", c->count_str);
	ft_free((void **)&c->count_str);
	if (!c->tmp_filename)
		return (-1);
	c->tmp_fd = open(c->tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (c->tmp_fd == -1)
	{
		perror("heredoc: open");
		return (-1);
	}
	return (0);
}

static int	hd_open_and_write(t_hd_ctx *c, t_mshell *mshell)
{
	if (hd_make_tmpfile(c) < 0)
		return (-1);
	hd_loop_write(c->tmp_fd, c->clean_delimiter, mshell, c->is_quoted);
	close(c->tmp_fd);
	return (0);
}

static int	hd_reopen_for_read(t_hd_ctx *c)
{
	c->read_fd = open(c->tmp_filename, O_RDONLY);
	if (c->read_fd == -1)
	{
		perror("heredoc: reopen");
		unlink(c->tmp_filename);
		return (-1);
	}
	unlink(c->tmp_filename);
	return (0);
}

int	handle_heredoc(const char *delimiter, t_mshell *mshell)
{
	static int	heredoc_count = 0;
	t_hd_ctx	c;

	(void)delimiter;
	c.read_fd = heredoc_count++;
	if (hd_prepare_ctx(&c, mshell) < 0)
		return (-1);
	if (hd_open_and_write(&c, mshell) < 0)
		return (ft_free((void **)&c.clean_delimiter), -1);
	if (hd_reopen_for_read(&c) < 0)
	{
		ft_free((void **)&c.tmp_filename);
		ft_free((void **)&c.clean_delimiter);
		return (-1);
	}
	ft_free((void **)&c.tmp_filename);
	ft_free((void **)&c.clean_delimiter);
	return (c.read_fd);
}
