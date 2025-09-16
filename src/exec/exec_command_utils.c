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
#include <errno.h>
#include "utils.h"

void	perror_exit(char *msg, int exit_code);

static void	exit_with_errno_message(const char *cmd)
{
	print_err2("minishell: ", cmd, ": ");
	print_err2(strerror(errno), "\n", NULL);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}

static void	execute_external_command(t_ent *node, t_envp **envp, char **env_arr)
{
	char	*path;
	char	**processed_argv;
	char	*cmd0;

	processed_argv = process_argv_quotes(node->argv);
	if (!processed_argv)
		exit(1);
	cmd0 = processed_argv[0];
	/* If command contains '/', try executing directly to let errno decide */
	if (cmd0 && ft_strchr(cmd0, '/'))
	{
		execve(cmd0, processed_argv, env_arr);
		/* execve failed: map errno to exit code per bash semantics */
		exit_with_errno_message(cmd0);
	}
	/* Otherwise search in PATH */
	path = find_command_path(cmd0, *envp);
	if (!path)
	{
		print_err2("minishell: ", cmd0, ": command not found\n");
		free_processed_argv(processed_argv);
		exit(127);
	}
	execve(path, processed_argv, env_arr);
	/* If we are here, execve failed with a found path: permission or similar */
	print_err2("minishell: ", cmd0, ": ");
	print_err2(strerror(errno), "\n", NULL);
	free(path);
	free_processed_argv(processed_argv);
	exit(126);
}

void	handle_child_process(t_ent *node, t_mshell *mshell, char **env_arr)
{
	// Restaurar señales a comportamiento por defecto en procesos hijos
	setup_child_signals();
	
	if (apply_redirections(node))
		exit(1);
	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node, mshell));
	execute_external_command(node, &(mshell->envp), env_arr);
}

int	wait_for_child_and_cleanup(pid_t pid, char **env_arr)
{
	int	status;

	set_child_executing();
	waitpid(pid, &status, 0);
	unset_child_executing();
	free_env_array(env_arr);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
