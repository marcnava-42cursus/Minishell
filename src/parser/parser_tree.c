/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/05 18:59:49 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

t_ent *parse_pipeline(const char **s)
{
	t_ent *head;
	t_ent *last;
	t_ent *op;
	t_ent *right;

	head = parse_primary(s);
	if (!head)
		return (NULL);
	last = head;
	while (last->next)
		last = last->next;
	skip_whitespace(s);
	while (**s == '|' && *(*s + 1) != '|')
	{
		(*s)++;
		op = ent_new_node(NODE_PIPE, NULL);
		last->next = op;
		last = op;
		skip_whitespace(s);
		right = parse_primary(s);
		last->next = right;
		while (last->next)
			last = last->next;
		skip_whitespace(s);
	}
	return (head);
}

t_ent *parse_and(const char **s)
{
	t_ent *head;
	t_ent *last;
	t_ent *op;
	t_ent *sub;

	head = parse_pipeline(s);
	if (!head)
		return (NULL);
	last = head;
	while (last->next)
		last = last->next;
	skip_whitespace(s);
	while (**s == '&' && *(*s + 1) == '&')
	{
		(*s) += 2;
		op = ent_new_node(NODE_AND, NULL);
		last->next = op;
		last = op;
		sub = parse_pipeline(s);
		last->next = sub;
		while (last->next)
			last = last->next;
		skip_whitespace(s);
	}
	return (head);
}

t_ent *parse_list(const char **s)
{
	t_ent *head;
	t_ent *last;
	t_ent *op;
	t_ent *sub;

	head = parse_and(s);
	if (!head)
		return (NULL);
	last = head;
	while (last->next)
		last = last->next;
	skip_whitespace(s);
	while (**s == '|' && *(*s + 1) == '|')
	{
		(*s) += 2;
		op = ent_new_node(NODE_OR, NULL);
		last->next = op;
		last = op;
		sub = parse_and(s);
		last->next = sub;
		while (last->next)
			last = last->next;
		skip_whitespace(s);
	}
	return (head);
}

t_ent *parse_primary(const char **s)
{
	skip_whitespace(s);
	if (**s == '(')
		return (parse_subshell(s));
	return (parse_cmd(s));
}

t_ent *parse_command_tree(const char *cmd)
{
	const char *p = cmd;
	return (parse_list(&p));
}
