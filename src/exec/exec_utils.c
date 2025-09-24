/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:41:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/24 21:31:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	match_builtin_name(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "env") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	return (0);
}

static void	free_string_array(char **arr)
{
	int		index;

	if (!arr)
		return ;
	index = 0;
	while (arr[index])
	{
		free(arr[index]);
		index++;
	}
	free(arr);
}

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
	int		result;

	if (!cmd)
		return (0);
	argv[0] = cmd;
	argv[1] = NULL;
	unquoted = process_argv_quotes(argv);
	if (!unquoted)
		return (0);
	result = match_builtin_name(unquoted[0]);
	free_string_array(unquoted);
	return (result);
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
