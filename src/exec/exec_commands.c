/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 04:12:34 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "utils.h"

int	exec_command(t_ent *node, t_mshell *mshell)
{
	pid_t	pid;
	char	**env_arr;
	char	**probe;

	if (!node->argv || !node->argv[0])
		return (0);
	/* Detect comando vacío tras quitar comillas (p.ej. $NOVAR => "") */
	probe = process_argv_quotes(node->argv);
	if (!probe)
		return (1);
	if (!probe[0] || probe[0][0] == '\0')
	{
		/* Bash: command not found => 127 y stderr no vacío */
		print_err2("minishell: ", ": command not found\n", NULL);
		free_processed_argv(probe);
		return (127);
	}
	free_processed_argv(probe);
	if (is_builtin(node->argv[0]) && node->fd_in == -1 && node->fd_out == -1)
		return (exec_builtin(node, mshell));
	env_arr = envp_to_array(mshell->envp);
	if (!env_arr)
		return (1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free_env_array(env_arr), 1);
	if (pid == 0)
		handle_child_process(node, mshell, env_arr);
	return (wait_for_child_and_cleanup(pid, env_arr));
}

static int	execute_pipeline_child(t_ent **commands, int **pipes,
t_mshell *mshell, int i, int cmd_count)
{
	// Restaurar señales a comportamiento por defecto en procesos hijos
	setup_child_signals();
	
	setup_input_redirection(pipes, commands[i], i);
	setup_output_redirection(pipes, commands[i], i, cmd_count);
	close_all_pipes(pipes, cmd_count);
	if (is_builtin(commands[i]->argv[0]))
		exit(exec_builtin(commands[i], mshell));
	else
		handle_child_process(commands[i], mshell, envp_to_array(mshell->envp));
	return (0);
}

int	exec_pipeline(t_ent *node, t_mshell *mshell)
{
	t_ent	**commands;
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		status;
	int		i;
	t_ent	*current;

	cmd_count = count_pipeline_commands(node);
	if (cmd_count == 0)
		return (1);
	if (cmd_count == 1)
	{
		current = node;
		while (current && current->type != NODE_COMMAND)
			current = current->next;
		return (exec_command(current, mshell));
	}
	commands = malloc(sizeof(t_ent *) * cmd_count);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!commands || !pipes || !pids)
		return (1);
	collect_pipeline_commands(node, commands, cmd_count);
	if (setup_pipeline_pipes(pipes, cmd_count))
		return (1);
	i = -1;
	while (++i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (perror("fork"), 1);
		if (pids[i] == 0)
			execute_pipeline_child(commands, pipes, mshell, i, cmd_count);
	}
	close_all_pipes(pipes, cmd_count);
	set_child_executing();
	i = -1;
	while (++i < cmd_count)
		waitpid(pids[i], &status, 0);
	unset_child_executing();
	cleanup_pipeline_resources(commands, pipes, pids, cmd_count);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_logic(t_ent *node, t_mshell *mshell)
{
	int			left;
	t_mshell	left_shell;
	t_mshell	right_shell;

	left_shell = *mshell;
	left_shell.tree = node->child;
	left = exec_tree(&left_shell);
	if (node->type == NODE_AND)
	{
		if (left == 0 && node->child->next)
		{
			right_shell = *mshell;
			right_shell.tree = node->child->next;
			return (exec_tree(&right_shell));
		}
		else
			return (left);
	}
	else if (node->type == NODE_OR)
	{
		if (left != 0 && node->child->next)
		{
			right_shell = *mshell;
			right_shell.tree = node->child->next;
			return (exec_tree(&right_shell));
		}
		else
			return (left);
	}
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
		// Restaurar señales a comportamiento por defecto en subshells
		setup_child_signals();
		
		if (apply_redirections(node))
			exit(1);
		child_shell = *mshell;
		child_shell.tree = node->child;
		exit(exec_tree(&child_shell));
	}
	set_child_executing();
	waitpid(pid, &status, 0);
	unset_child_executing();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_builtin(t_ent *node, t_mshell *mshell)
{
	char	*cmd;
	char	**processed_argv;
	int		result;

	if (!node || !node->argv || !node->argv[0])
		return (1);
	processed_argv = process_argv_quotes(node->argv);
	if (!processed_argv)
		return (1);
	cmd = processed_argv[0];
	result = 1;
	if (ft_strcmp(cmd, "cd") == 0)
		result = handle_cd_builtin(processed_argv, &(mshell->envp));
	else if (ft_strcmp(cmd, "echo") == 0)
		result = handle_echo_builtin(node, processed_argv);
	else if (ft_strcmp(cmd, "env") == 0)
		result = msh_exec_bt_env(mshell->envp);
	else if (ft_strcmp(cmd, "exit") == 0)
		result = handle_exit_builtin(processed_argv, mshell->exit_code);
	else if (ft_strcmp(cmd, "export") == 0)
		result = handle_export_builtin(processed_argv, &(mshell->envp));
	else if (ft_strcmp(cmd, "pwd") == 0)
		result = msh_exec_bt_pwd(mshell->envp);
	else if (ft_strcmp(cmd, "unset") == 0)
		result = handle_unset_builtin(processed_argv, &(mshell->envp));
	free_processed_argv(processed_argv);
	return (result);
}
