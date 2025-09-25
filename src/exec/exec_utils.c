/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:41:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/26 00:29:47 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	*dup_last_arg(char **argv)
{
	char	**unq;
	int		c;
	char	*res;

	if (!argv || !argv[0])
		return (NULL);
	c = 0;
	while (argv[c])
		c++;
	if (c == 0)
		return (NULL);
	unq = process_argv_quotes(argv);
	if (!unq)
		return (NULL);
	res = ft_strdup(unq[c - 1]);
	ft_free_matrix((void **)unq);
	return (res);
}

char	*dup_last_export_assign(char **argv)
{
	char	**unq;
	int		i;
	char	*dup;

	if (!argv || !argv[0])
		return (NULL);
	unq = process_argv_quotes(argv);
	if (!unq)
		return (NULL);
	if (!unq[0] || ft_strcmp(unq[0], "export") != 0)
		return (ft_free_matrix((void **)unq), NULL);
	dup = NULL;
	i = 1;
	while (unq[i])
	{
		if (ft_strchr(unq[i], '='))
		{
			ft_free((void **)&dup);
			dup = ft_strdup(unq[i]);
		}
		i++;
	}
	ft_free_matrix((void **)unq);
	return (dup);
}

char	*dup_from_pipeline_commands(t_ent **cmds, int n)
{
	int		i;
	char	*assign;
	char	*fallback;

	assign = NULL;
	i = 0;
	while (i < n)
	{
		assign = dup_last_export_assign(cmds[i]->argv);
		if (assign)
			return (assign);
		i++;
	}
	fallback = dup_last_arg(cmds[n - 1]->argv);
	return (fallback);
}

/**
 * @brief Apply redirections for a node
 * 
 * @param node The node containing redirection information
 * @return 0 on success, 1 on error
 */
int	apply_redirections(t_ent *node)
{
	if (node->fd_in == -2 || node->fd_out == -2)
		return (1);
	if (node->fd_in != -1)
	{
		if (dup2(node->fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"), 1);
		close(node->fd_in);
	}
	if (node->fd_out != -1)
	{
		if (dup2(node->fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);
		close(node->fd_out);
	}
	return (0);
}
