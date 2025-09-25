/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 06:50:32 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "utils.h"

void	exit_with_errno_message(const char *cmd);
void	execute_external_command(t_ent *node, t_envp **envp, char **env_arr);

void	handle_child_process(t_ent *node, t_mshell *mshell, char **env_arr)
{
	set_child_signal();
	if (apply_redirections(node))
		exit(1);
	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node, mshell));
	execute_external_command(node, &(mshell->envp), env_arr);
}

int	wait_for_child_and_cleanup(pid_t pid, t_mshell *mshell, char **env_arr)
{
	int	status;
	int	sig;

	block_parent_signals(mshell);
	waitpid(pid, &status, 0);
	restore_parent_signals(mshell);
	ft_free_matrix((void **)env_arr);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
		{
			if (WCOREDUMP(status))
				print_err2("Quit (core dumped)\n", NULL, NULL);
		}
		return (128 + sig);
	}
	return (1);
}

void	check_handler_process(pid_t pid, char **env_arr, t_ent *node,
	t_mshell *mshell)
{
	if (pid == 0)
		handle_child_process(node, mshell, env_arr);
	if (node->fd_in != -1 && node->fd_in != -2)
	{
		close(node->fd_in);
		node->fd_in = -1;
	}
	if (node->fd_out != -1 && node->fd_out != -2)
	{
		close(node->fd_out);
		node->fd_out = -1;
	}
}
