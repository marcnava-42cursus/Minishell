/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:41:11 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 23:41:11 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include "utils.h"

void	exit_with_errno_message(const char *cmd);

static void	try_exec_direct(char *cmd0, char **argv, char **env_arr)
{
	if (cmd0 && ft_strchr(cmd0, '/'))
	{
		execve(cmd0, argv, env_arr);
		exit_with_errno_message(cmd0);
	}
}

static char	*resolve_path_or_exit(const char *cmd0, t_envp *envp)
{
	char	*path;

	path = find_command_path((char *)cmd0, envp);
	if (!path)
	{
		print_err2("minishell: ", cmd0, ": command not found\n");
		exit(127);
	}
	return (path);
}

void	execute_external_command(t_ent *node, t_envp **envp, char **env_arr)
{
	char	*path;
	char	**processed_argv;
	char	*cmd0;

	processed_argv = process_argv_quotes(node->argv);
	if (!processed_argv)
		exit(1);
	cmd0 = processed_argv[0];
	try_exec_direct(cmd0, processed_argv, env_arr);
	path = resolve_path_or_exit(cmd0, *envp);
	execve(path, processed_argv, env_arr);
	print_err2("minishell: ", cmd0, ": ");
	print_err2(strerror(errno), "\n", NULL);
	ft_free((void **)&path);
	ft_free_matrix((void **)processed_argv);
	exit(126);
}
