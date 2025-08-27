/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 02:45:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	perror_exit(char *msg, int exit_code);

static void	execute_external_command(t_ent *node, t_envp **envp, char **env_arr)
{
	char	*path;
	char	**processed_argv;

	path = find_command_path(node->argv[0], *envp);
	if (!path)
	{
		printf("minishell: %s: command not found\n", node->argv[0]);
		exit(127);
	}
	processed_argv = process_argv_quotes(node->argv);
	if (!processed_argv)
	{
		free(path);
		exit(1);
	}
	execve(path, processed_argv, env_arr);
	perror("execve");
	free(path);
	free_processed_argv(processed_argv);
	exit(126);
}

void	handle_child_process(t_ent *node, t_envp **envp, char **env_arr)
{
	if (apply_redirections(node))
		exit(1);
	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node, envp, NULL));
	execute_external_command(node, envp, env_arr);
}

int	wait_for_child_and_cleanup(pid_t pid, char **env_arr)
{
	int	status;

	waitpid(pid, &status, 0);
	free_env_array(env_arr);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
