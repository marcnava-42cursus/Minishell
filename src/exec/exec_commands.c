/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 02:05:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	exec_command(t_ent *node, t_envp **envp, t_config *config)
{
	pid_t	pid;
	char	**env_arr;

	if (!node->argv || !node->argv[0])
		return (0);
	if (is_builtin(node->argv[0]) && node->fd_in == -1 && node->fd_out == -1)
		return (exec_builtin(node, envp, config));
	env_arr = envp_to_array(*envp);
	if (!env_arr)
		return (1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free_env_array(env_arr), 1);
	if (pid == 0)
		handle_child_process(node, envp, env_arr);
	return (wait_for_child_and_cleanup(pid, env_arr));
	(void)config;
}

static int	execute_pipeline_child(t_ent **commands, int **pipes, 
	t_envp **envp, t_config *config, int i, int cmd_count)
{
	setup_input_redirection(pipes, commands[i], i);
	setup_output_redirection(pipes, commands[i], i, cmd_count);
	close_all_pipes(pipes, cmd_count);
	if (is_builtin(commands[i]->argv[0]))
		exit(exec_builtin(commands[i], envp, config));
	else
		handle_child_process(commands[i], envp, envp_to_array(*envp));
	return (0);
}

int	exec_pipeline(t_ent *node, t_envp **envp, t_config *config)
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
		return (exec_command(current, envp, config));
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
			execute_pipeline_child(commands, pipes, envp, config, i, cmd_count);
	}
	i = -1;
	while (++i < cmd_count)
		waitpid(pids[i], &status, 0);
	cleanup_pipeline_resources(commands, pipes, pids, cmd_count);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_logic(t_ent *node, t_envp **envp, t_config *config)
{
	int left = exec_tree(node->child, *envp, config);
	if (node->type == NODE_AND)
	{
		if (left == 0)
			return (exec_tree(node->child->next, *envp, config));
		else
			return (left);
	}
	else if (node->type == NODE_OR)
	{
		if (left != 0)
			return (exec_tree(node->child->next, *envp, config));
		else
			return (left);
	}
	return (1);
}

int	exec_subshell(t_ent *node, t_envp **envp, t_config *config)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (apply_redirections(node))
			exit(1);
		exit(exec_tree(node->child, *envp, config));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_builtin(t_ent *node, t_envp **envp, t_config *config)
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
		result = handle_cd_builtin(processed_argv, envp);
	else if (ft_strcmp(cmd, "echo") == 0)
		result = handle_echo_builtin(node, processed_argv);
	else if (ft_strcmp(cmd, "env") == 0)
		result = msh_exec_bt_env(*envp);
	else if (ft_strcmp(cmd, "exit") == 0)
		result = msh_exec_bt_exit();
	else if (ft_strcmp(cmd, "export") == 0)
		result = handle_export_builtin(processed_argv, envp);
	else if (ft_strcmp(cmd, "pwd") == 0)
		result = msh_exec_bt_pwd(*envp);
	else if (ft_strcmp(cmd, "unset") == 0)
		result = handle_unset_builtin(processed_argv, envp);
	free_processed_argv(processed_argv);
	(void)config;
	return (result);
}
