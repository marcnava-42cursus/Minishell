/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 23:13:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "utils.h"

static int	validate_command_name(char **argv)
{
	char	**probe;

	probe = process_argv_quotes(argv);
	if (!probe)
		return (-1);
	if (!probe[0] || probe[0][0] == '\0')
	{
		print_err2("minishell: ", ": command not found\n", NULL);
		ft_free_matrix((void **)probe);
		return (127);
	}
	ft_free_matrix((void **)probe);
	return (0);
}

int	exec_command(t_ent *node, t_mshell *mshell)
{
	pid_t	pid;
	char	**env_arr;
	int		ok;

	if (!node->argv || !node->argv[0])
		return (0);
	ok = validate_command_name(node->argv);
	if (ok != 0)
	{
		if (ok == -1)
			return (1);
		return (ok);
	}
	if (is_builtin(node->argv[0]) && node->fd_in == -1 && node->fd_out == -1)
		return (exec_builtin(node, mshell));
	env_arr = envp_to_array(mshell->envp);
	if (!env_arr)
		return (1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), ft_free_matrix((void **)env_arr), 1);
	if (pid == 0)
		handle_child_process(node, mshell, env_arr);
	return (wait_for_child_and_cleanup(pid, env_arr));
}

static int	builtin_dispatch(const char *cmd, t_ent *node,
				char **argv, t_mshell *mshell)
{
	if (ft_strcmp((char *)cmd, "cd") == 0)
		return (handle_cd_builtin(argv, &(mshell->envp)));
	else if (ft_strcmp((char *)cmd, "echo") == 0)
		return (handle_echo_builtin(node, argv));
	else if (ft_strcmp((char *)cmd, "env") == 0)
		return (msh_exec_bt_env(argv, mshell->envp));
	else if (ft_strcmp((char *)cmd, "exit") == 0)
		return (handle_exit_builtin(argv, mshell->exit_code));
	else if (ft_strcmp((char *)cmd, "export") == 0)
		return (handle_export_builtin(argv, &(mshell->envp)));
	else if (ft_strcmp((char *)cmd, "pwd") == 0)
		return (msh_exec_bt_pwd(mshell->envp));
	else if (ft_strcmp((char *)cmd, "unset") == 0)
		return (handle_unset_builtin(argv, &(mshell->envp)));
	return (1);
}

int	exec_subshell(t_ent *node, t_mshell *mshell)
{
	pid_t		pid;
	int			status;
	t_mshell	child_shell;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		reset_signals_to_default();
		if (apply_redirections(node))
			exit(1);
		child_shell = *mshell;
		child_shell.tree = node->child;
		exit(exec_tree(&child_shell));
	}
	g_child_executing = 1;
	waitpid(pid, &status, 0);
	g_child_executing = 0;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	exec_builtin(t_ent *node, t_mshell *mshell)
{
	char	**processed_argv;
	int		result;

	if (!node || !node->argv || !node->argv[0])
		return (1);
	processed_argv = process_argv_quotes(node->argv);
	if (!processed_argv)
		return (1);
	result = builtin_dispatch(processed_argv[0], node, processed_argv, mshell);
	ft_free_matrix((void **)processed_argv);
	return (result);
}
