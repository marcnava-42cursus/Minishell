/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 17:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	advance_to_last(t_ent **node)
{
	while ((*node)->next)
		*node = (*node)->next;
}

void	handle_parse_error(t_ent *node, t_mshell *mshell)
{
	ent_free(node);
	if (mshell->exit_code == 0)
		mshell->exit_code = 2;
}

t_ent	*handle_pipe_parsing(const char **s, t_mshell *mshell,
				t_ent *head, t_ent *last)
{
	t_ent	*rhs;

	while (**s == '|' && *(*s + 1) != '|')
	{
		(*s)++;
		last->next = ent_new_node(NODE_PIPE, NULL);
		last = last->next;
		parser_skip_whitespace(s);
		rhs = parse_primary(s, mshell);
		if (!rhs)
		{
			handle_parse_error(head, mshell);
			return (NULL);
		}
		last->next = rhs;
		advance_to_last(&last);
		parser_skip_whitespace(s);
	}
	return (head);
}

t_ent	*handle_and_parsing(const char **s, t_mshell *mshell, t_ent *left)
{
	t_ent	*right;
	t_ent	*parent;

	while (**s == '&' && *(*s + 1) == '&')
	{
		(*s) += 2;
		right = parse_pipeline(s, mshell);
		if (!right)
		{
			handle_parse_error(left, mshell);
			return (NULL);
		}
		parent = ent_new_node(NODE_AND, NULL);
		ent_new_child(parent, left);
		ent_new_child(parent, right);
		left = parent;
		parser_skip_whitespace(s);
	}
	return (left);
}

t_ent	*handle_or_parsing(const char **s, t_mshell *mshell, t_ent *left)
{
	t_ent	*right;
	t_ent	*parent;

	while (**s == '|' && *(*s + 1) == '|')
	{
		(*s) += 2;
		right = parse_and(s, mshell);
		if (!right)
		{
			handle_parse_error(left, mshell);
			return (NULL);
		}
		parent = ent_new_node(NODE_OR, NULL);
		ent_new_child(parent, left);
		ent_new_child(parent, right);
		left = parent;
		parser_skip_whitespace(s);
	}
	return (left);
}
