/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 00:55:08 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	advance_to_last(t_ent **node)
{
	while ((*node)->next)
		*node = (*node)->next;
}

t_ent	*parse_pipeline(const char **s, t_mshell *mshell)
{
	t_ent	*head;
	t_ent	*last;
	t_ent	*rhs;

	head = parse_primary(s, mshell);
	if (!head)
		return (NULL);
	last = head;
	advance_to_last(&last);
	parser_skip_whitespace(s);
	while (**s == '|' && *(*s + 1) != '|')
	{
		(*s)++;
		last->next = ent_new_node(NODE_PIPE, NULL);
		last = last->next;
		parser_skip_whitespace(s);
		rhs = parse_primary(s, mshell);
		if (!rhs)
		{
			/* Error después de un pipe: liberar y propagar fallo */
			ent_free(head);
			mshell->exit_code = (mshell->exit_code == 0) ? 2 : mshell->exit_code;
			return (NULL);
		}
		last->next = rhs;
		advance_to_last(&last);
		parser_skip_whitespace(s);
	}
	return (head);
}

t_ent	*parse_and(const char **s, t_mshell *mshell)
{
	t_ent	*left;
	t_ent	*right;
	t_ent	*parent;

	left = parse_pipeline(s, mshell);
	if (!left)
		return (NULL);
	parser_skip_whitespace(s);
	while (**s == '&' && *(*s + 1) == '&')
	{
		(*s) += 2;
		right = parse_pipeline(s, mshell);
		if (!right)
		{
			ent_free(left);
			mshell->exit_code = (mshell->exit_code == 0) ? 2 : mshell->exit_code;
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

t_ent	*parse_list(const char **s, t_mshell *mshell)
{
	t_ent	*left;
	t_ent	*right;
	t_ent	*parent;

	left = parse_and(s, mshell);
	if (!left)
		return (NULL);
	parser_skip_whitespace(s);
	while (**s == '|' && *(*s + 1) == '|')
	{
		(*s) += 2;
		right = parse_and(s, mshell);
		if (!right)
		{
			ent_free(left);
			mshell->exit_code = (mshell->exit_code == 0) ? 2 : mshell->exit_code;
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

t_ent	*parse_primary(const char **s, t_mshell *mshell)
{
	parser_skip_whitespace(s);
	if (**s == '(')
		return (parse_subshell(s, mshell));
	return (parse_cmd(s, mshell));
}

t_ent	*parse_command_tree(const char *cmd, t_mshell *mshell)
{
	const char	*p;

	p = cmd;
	return (parse_list(&p, mshell));
}
