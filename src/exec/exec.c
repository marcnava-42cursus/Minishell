/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:23:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/07 18:41:42 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	exec_tree(t_ent *node, t_envp *envp, t_config *config)
{
	t_ent	*current;
	int		has_pipe;

	if (!node)
		return (0);

	// Check if this sequence contains any pipes
	has_pipe = 0;
	current = node;
	while (current)
	{
		if (current->type == NODE_PIPE)
		{
			has_pipe = 1;
			break;
		}
		current = current->next;
	}

	// If there are pipes in this sequence, handle the whole pipeline
	if (has_pipe)
	{
		config->exit_code = exec_pipeline(node, &envp, config);
		return (config->exit_code);
	}

	// Handle single nodes
	if (node->type == NODE_COMMAND)
		config->exit_code = exec_command(node, &envp, config);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		config->exit_code = exec_logic(node, &envp, config);
	else if (node->type == NODE_SUBSHELL)
		config->exit_code = exec_subshell(node, &envp, config);
	else
	{
		printf("minishell: unsupported operation\n");
		config->exit_code = 1;
	}
	return (config->exit_code);
}

/* ************************************************************************** */
/*   exec_command.c                                                          */
/* ************************************************************************** */

int	apply_redirections(t_ent *node)
{
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

int	exec_command(t_ent *node, t_envp **envp, t_config *config)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**env_arr;

	if (!node->argv || !node->argv[0])
		return (0);
	if (is_builtin(node->argv[0]) && node->fd_in == -1 && node->fd_out == -1)
		return (execute_builtin(node, envp, config));
	env_arr = envp_to_array(*envp);
	if (!env_arr)
		return (1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free_env_array(env_arr), 1);
	if (pid == 0)
	{
		if (apply_redirections(node))
			exit(1);
		if (is_builtin(node->argv[0]))
			exit(execute_builtin(node, envp, config));
		path = find_command_path(node->argv[0], *envp);
		if (!path)
		{
			printf("minishell: %s: command not found\n", node->argv[0]);
			exit(127);
		}
		execve(path, node->argv, env_arr);
		perror("execve");
		exit(126);
	}
	waitpid(pid, &status, 0);
	free_env_array(env_arr);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/* ************************************************************************** */
/*   exec_pipeline.c                                                         */
/* ************************************************************************** */

int	exec_pipeline(t_ent *node, t_envp **envp, t_config *config)
{
	t_ent	**commands;
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		status;
	int		i;
	t_ent	*current;

	// Count commands in the pipeline
	cmd_count = 0;
	current = node;
	while (current)
	{
		if (current->type == NODE_COMMAND)
			cmd_count++;
		current = current->next;
	}

	if (cmd_count == 0)
		return (1);
	if (cmd_count == 1)
	{
		// Single command, no pipes needed
		current = node;
		while (current && current->type != NODE_COMMAND)
			current = current->next;
		return (exec_command(current, envp, config));
	}

	// Allocate arrays
	commands = malloc(sizeof(t_ent *) * cmd_count);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!commands || !pipes || !pids)
		return (1);

	// Collect all commands
	i = 0;
	current = node;
	while (current && i < cmd_count)
	{
		if (current->type == NODE_COMMAND)
			commands[i++] = current;
		current = current->next;
	}

	// Create pipes
	for (i = 0; i < cmd_count - 1; i++)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
	}

	// Fork and execute each command
	for (i = 0; i < cmd_count; i++)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			return (1);
		}

		if (pids[i] == 0)
		{
			// Child process
			
			// Set up input (from previous pipe or stdin)
			if (i > 0)
			{
				if (dup2(pipes[i-1][0], STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(1);
				}
			}
			else if (commands[i]->fd_in != -1)
			{
				if (dup2(commands[i]->fd_in, STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(1);
				}
			}

			// Set up output (to next pipe or stdout)
			if (i < cmd_count - 1)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
				{
					perror("dup2");
					exit(1);
				}
			}
			else if (commands[i]->fd_out != -1)
			{
				if (dup2(commands[i]->fd_out, STDOUT_FILENO) == -1)
				{
					perror("dup2");
					exit(1);
				}
			}

			// Close all pipe file descriptors
			for (int j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			// Execute the command
			exit(exec_command(commands[i], envp, config));
		}
	}

	// Parent process: close all pipes and wait for children
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}

	// Wait for all children
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
	}

	// Cleanup
	free(commands);
	free(pipes);
	free(pids);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/* ************************************************************************** */
/*   exec_logic.c                                                            */
/* ************************************************************************** */

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

/* ************************************************************************** */
/*   exec_subshell.c                                                         */
/* ************************************************************************** */

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
