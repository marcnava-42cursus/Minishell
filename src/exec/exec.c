/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:23:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 04:09:08 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	exec_tree(t_mshell *mshell)
{
	t_ent	*current;
	t_ent	*node;
	int		has_pipe;

	node = mshell->tree;
	if (!node)
		return (0);
	has_pipe = 0;
	current = node;
	while (current)
	{
		if (current->type == NODE_PIPE)
		{
			has_pipe = 1;
			break ;
		}
		current = current->next;
	}
	if (has_pipe)
	{
		mshell->exit_code = exec_pipeline(node, mshell);
		return (mshell->exit_code);
	}
	if (node->type == NODE_COMMAND)
		mshell->exit_code = exec_command(node, mshell);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		mshell->exit_code = exec_logic(node, mshell);
	else if (node->type == NODE_SUBSHELL)
		mshell->exit_code = exec_subshell(node, mshell);
	else
	{
		printf("minishell: unsupported operation\n");
		mshell->exit_code = 1;
	}
	return (mshell->exit_code);
}
