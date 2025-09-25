/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:23:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 06:18:12 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "signals.h"
#include <sys/wait.h>
#include <stdlib.h>

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

static int	hd_fork_and_write(t_hd_ctx *c, t_mshell *mshell)
{
	pid_t	pid;
	int		status;

	if (hd_make_tmpfile(c) < 0)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("heredoc: fork");
		return (close(c->tmp_fd), unlink(c->tmp_filename), -1);
	}
	if (pid == 0)
	{
		set_heredoc_signal();
		hd_loop_write(c->tmp_fd, c->clean_delimiter, mshell, c->is_quoted);
		close(c->tmp_fd);
		exit(0);
	}
	block_parent_signals(mshell);
	close(c->tmp_fd);
	waitpid(pid, &status, 0);
	restore_parent_signals(mshell);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (130);
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
	int			w;

	c.read_fd = heredoc_count++;
	if (hd_prepare_ctx(&c, delimiter) < 0)
		return (-1);
	{
		w = hd_fork_and_write(&c, mshell);
		if (w < 0)
			return (ft_free((void **)&c.clean_delimiter), -1);
		if (w == 130)
		{
			unlink(c.tmp_filename);
			ft_free((void **)&c.tmp_filename);
			ft_free((void **)&c.clean_delimiter);
			mshell->exit_code = 130;
			return (-1);
		}
	}
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
