/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 00:22:25 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/26 00:33:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	match_builtin_name(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp((char *)name, "cd") == 0)
		return (1);
	if (ft_strcmp((char *)name, "echo") == 0)
		return (1);
	if (ft_strcmp((char *)name, "env") == 0)
		return (1);
	if (ft_strcmp((char *)name, "exit") == 0)
		return (1);
	if (ft_strcmp((char *)name, "export") == 0)
		return (1);
	if (ft_strcmp((char *)name, "pwd") == 0)
		return (1);
	if (ft_strcmp((char *)name, "unset") == 0)
		return (1);
	return (0);
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
	ft_free_matrix((void **)unquoted);
	return (result);
}
