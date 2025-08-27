/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 13:42:56 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static void	print_indent(int indent)
{
	int	i;

	i = 0;
	while (i++ < indent)
		write(1, "    ", 4);
}

/*
 * Print node type and arguments:
 * - SUBSHELL: print [SUBSHELL] and its argv[0]
 * - COMMAND:  print [COMMAND] and all argv[]
 * - PIPE/AND/OR: print marker only
 */
static void	print_node_info(t_ent *node)
{
	int	i;

	if (node->type == NODE_SUBSHELL)
	{
		printf("[SUBSHELL]");
		if (node->argv && node->argv[0])
			printf(" %s", node->argv[0]);
	}
	else if (node->type == NODE_COMMAND)
	{
		printf("[COMMAND]");
		i = 0;
		while (node->argv && node->argv[i])
			printf(" %s", node->argv[i++]);
	}
	else if (node->type == NODE_PIPE)
		printf("[PIPE]");
	else if (node->type == NODE_AND)
		printf("[AND]");
	else if (node->type == NODE_OR)
		printf("[OR]");
}

/*
 * Recursively print tree structure
 */
void	print_tree(t_ent *node, int indent)
{
	t_ent	*cur;

	cur = node;
	while (cur)
	{
		print_indent(indent);
		print_node_info(cur);
		printf("\n");
		if (cur->type == NODE_SUBSHELL && cur->child)
			print_tree(cur->child, indent + 1);
		cur = cur->next;
	}
}
