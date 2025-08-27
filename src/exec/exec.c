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

int	exec_tree(t_ent *node, t_envp *envp, t_config *config)
{
	t_ent	*current;
	int		has_pipe;

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
		config->exit_code = exec_pipeline(node, &envp, config);
		return (config->exit_code);
	}
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
