/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 18:22:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
		{
			perror("pipe");
			return (1);
		}
		i++;
	}
	return (0);
}

void	setup_input_redirection(int **pipes, t_ent *command, int i)
{
	if (command->fd_in != -1)
	{
		if (dup2(command->fd_in, STDIN_FILENO) == -1)
			perror("dup2");
		else
			close(command->fd_in);
		return ;
	}
	if (i > 0 && dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		perror("dup2");
}

void	setup_output_redirection(int **pipes, t_ent *command,
	int i, int cmd_count)
{
	if (command->fd_out != -1)
	{
		if (dup2(command->fd_out, STDOUT_FILENO) == -1)
			perror("dup2");
		else
			close(command->fd_out);
		return ;
	}
	if (i < cmd_count - 1 && dup2(pipes[i][1], STDOUT_FILENO) == -1)
		perror("dup2");
}
