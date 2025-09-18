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

static int	has_pipeline(t_ent *node)
{
	while (node)
	{
		if (node->type == NODE_PIPE)
			return (1);
		node = node->next;
	}
	return (0);
}

static int	dispatch_root(t_ent *node, t_mshell *mshell)
{
	if (node->type == NODE_COMMAND)
		return (exec_command(node, mshell));
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (exec_logic(node, mshell));
	if (node->type == NODE_SUBSHELL)
		return (exec_subshell(node, mshell));
	printf("minishell: unsupported operation\n");
	return (1);
}

int	exec_tree(t_mshell *mshell)
{
	t_ent	*node;

	node = mshell->tree;
	if (!node)
		return (0);
	if (has_pipeline(node))
		mshell->exit_code = exec_pipeline(node, mshell);
	else
		mshell->exit_code = dispatch_root(node, mshell);
	return (mshell->exit_code);
}
