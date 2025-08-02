/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/02 13:15:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/* Print indent (4 spaces per level) */
static void print_indent(int indent)
{
	int i;

	i = 0;
	while (i++ < indent)
		write(1, "    ", 4);
}

/*
 * Recursively print tree:
 * - SUBSHELL: print [SUBSHELL] and its argv[0]
 * - COMMAND:  print [COMMAND] and all argv[]
 * - PIPE/AND/OR: print marker only
 */
void print_tree(t_ent *node, int indent)
{
	t_ent *cur;
	int   i;

	cur = node;
	while (cur)
	{
		print_indent(indent);
		if (cur->type == NODE_SUBSHELL)
		{
			printf("[SUBSHELL]");
			if (cur->argv && cur->argv[0])
				printf(" %s", cur->argv[0]);
		}
		else if (cur->type == NODE_COMMAND)
		{
			printf("[COMMAND]");
			i = 0;
			while (cur->argv && cur->argv[i])
				printf(" %s", cur->argv[i++]);
		}
		else if (cur->type == NODE_PIPE)
			printf("[PIPE]");
		else if (cur->type == NODE_AND)
			printf("[AND]");
		else if (cur->type == NODE_OR)
			printf("[OR]");
		printf("\n");
		if (cur->type == NODE_SUBSHELL && cur->child)
			print_tree(cur->child, indent + 1);
		cur = cur->next;
	}
}
