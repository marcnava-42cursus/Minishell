/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_run.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:10:09 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/19 00:10:09 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	single_command_from_pipeline(t_ent *node, t_mshell *mshell)
{
	t_ent	*current;

	current = node;
	while (current && current->type != NODE_COMMAND)
		current = current->next;
	if (!current)
		return (1);
	return (exec_command(current, mshell));
}

int	spawn_children(t_pipe_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->cmd_count)
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] == -1)
			return (perror("fork"), 1);
		if (ctx->pids[i] == 0)
			exec_pipeline_child(ctx, i);
		i++;
	}
	return (0);
}
