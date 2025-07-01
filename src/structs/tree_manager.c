/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:26:20 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/30 20:45:58 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

t_ent	*ent_new_node(t_node_type type, char **argv)
{
	t_ent	*node;

	node = ft_calloc(1, sizeof(t_ent));
	if (!node)
		return (NULL);
	node->type = type;
	node->argv = argv;
	node->fd_in = -1;
	node->fd_out = -1;
	node->child = NULL;
	node->next = NULL;
	return (node);
}

int	ent_new_sibling(t_ent *node, t_ent *sibling)
{
	t_ent	*tmp;

	if (!node || !sibling)
		return (1);
	tmp = node;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = sibling;
	return (0);
}

int	ent_new_child(t_ent *parent, t_ent *child)
{
	t_ent	*tmp;

	if (!parent || !child)
		return (1);
	if (!parent->child)
		parent->child = child;
	else
	{
		tmp = parent->child;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = child;
	}
	return (0);
}

void	ent_free_node(t_ent *node)
{
	int	i;

	if (!node)
		return ;
	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
			ft_free((void **)&node->argv[i++]);
		ft_free((void **)&node->argv);
	}
	ft_free((void **)&node);
}

void	ent_free(t_ent *root)
{
	if (!root)
		return ;
	ent_free(root->child);
	ent_free(root->next);
	ent_free_node(root);
}
