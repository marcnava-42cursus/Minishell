/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 05:37:52 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "utils.h"

void	perror_exit(char *msg, int exit_code);
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

int	wait_for_child_and_cleanup(pid_t pid, char **env_arr)
{
	int	status;
	struct sigaction old_int;
	struct sigaction old_quit;

	block_parent_signals(&old_int, &old_quit);
	waitpid(pid, &status, 0);
	restore_parent_signals(&old_int, &old_quit);
	ft_free_matrix((void **)env_arr);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
