/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:48:20 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 23:48:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	exec_logic_and(t_ent *node, t_mshell *mshell, int left)
{
	t_mshell	right_shell;

	if (left == 0 && node->child->next)
	{
		right_shell = *mshell;
		right_shell.tree = node->child->next;
		return (exec_tree(&right_shell));
	}
	return (left);
}

static int	exec_logic_or(t_ent *node, t_mshell *mshell, int left)
{
	t_mshell	right_shell;

	if (left != 0 && node->child->next)
	{
		right_shell = *mshell;
		right_shell.tree = node->child->next;
		return (exec_tree(&right_shell));
	}
	return (left);
}

int	exec_logic(t_ent *node, t_mshell *mshell)
{
	int			left;
	t_mshell	left_shell;

	left_shell = *mshell;
	left_shell.tree = node->child;
	left = exec_tree(&left_shell);
	if (node->type == NODE_AND)
		return (exec_logic_and(node, mshell, left));
	if (node->type == NODE_OR)
		return (exec_logic_or(node, mshell, left));
	return (1);
}
