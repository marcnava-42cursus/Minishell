/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:41:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 20:10:44 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/**
 * @brief Check if a command is a builtin
 * 
 * @param cmd The command to check
 * @return 1 if it's a builtin, 0 otherwise
 */
int	is_builtin(char *cmd)
{
	char	*argv[2];
	char	**unquoted;
	int		is_bt;
	int		i;

	if (!cmd)
		return (0);
	argv[0] = cmd;
	argv[1] = NULL;
	unquoted = process_argv_quotes(argv);
	if (!unquoted)
		return (0);
	is_bt = 0;
	if (unquoted[0])
	{
		if (ft_strcmp(unquoted[0], "cd") == 0)
			is_bt = 1;
		else if (ft_strcmp(unquoted[0], "echo") == 0)
			is_bt = 1;
		else if (ft_strcmp(unquoted[0], "env") == 0)
			is_bt = 1;
		else if (ft_strcmp(unquoted[0], "exit") == 0)
			is_bt = 1;
		else if (ft_strcmp(unquoted[0], "export") == 0)
			is_bt = 1;
		else if (ft_strcmp(unquoted[0], "pwd") == 0)
			is_bt = 1;
		else if (ft_strcmp(unquoted[0], "unset") == 0)
			is_bt = 1;
	}
	i = 0;
	while (unquoted[i])
		free(unquoted[i++]);
	free(unquoted);
	return (is_bt);
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
