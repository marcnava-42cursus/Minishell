/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 04:11:50 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	perror_exit(char *msg, int exit_code)
{
	perror(msg);
	exit(exit_code);
}

int	count_pipeline_commands(t_ent *node)
{
	t_ent	*current;
	int		cmd_count;

	cmd_count = 0;
	current = node;
	while (current)
	{
		if (current->type == NODE_COMMAND)
			cmd_count++;
		current = current->next;
	}
	return (cmd_count);
}

void	collect_pipeline_commands(t_ent *node, t_ent **commands, int cmd_count)
{
	t_ent	*current;
	int		i;

	i = 0;
	current = node;
	while (current && i < cmd_count)
	{
		if (current->type == NODE_COMMAND)
			commands[i++] = current;
		current = current->next;
	}
}

int	setup_pipeline_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
			return (perror("pipe"), 1);
		i++;
	}
	return (0);
}

void	setup_input_redirection(int **pipes, t_ent *command, int i)
{
	if (i > 0 && dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		perror_exit("dup2", 1);
	if (i == 0 && command->fd_in != -1
		&& dup2(command->fd_in, STDIN_FILENO) == -1)
		perror_exit("dup2", 1);
}

void	setup_output_redirection(int **pipes, t_ent *command,
	int i, int cmd_count)
{
	if (i < cmd_count - 1 && dup2(pipes[i][1], STDOUT_FILENO) == -1)
		perror_exit("dup2", 1);
	if (i == cmd_count - 1 && command->fd_out != -1
		&& dup2(command->fd_out, STDOUT_FILENO) == -1)
		perror_exit("dup2", 1);
}

void	close_all_pipes(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void	cleanup_pipeline_resources(t_ent **commands, int **pipes, pid_t *pids,
	int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(commands);
	free(pipes);
	free(pids);
}
