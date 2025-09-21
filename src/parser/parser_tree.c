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

t_ent	*parse_pipeline(const char **s, t_mshell *mshell)
{
	t_ent	*head;
	t_ent	*last;

	head = parse_primary(s, mshell);
	if (!head)
		return (NULL);
	last = head;
	while (last->next)
		last = last->next;
	parser_skip_whitespace(s);
	while (**s == '|' && *(*s + 1) != '|')
		pt_attach_pipe(s, mshell, &last);
	return (head);
}

t_ent	*parse_and(const char **s, t_mshell *mshell)
{
	t_ent	*head;
	t_ent	*last;

	head = parse_pipeline(s, mshell);
	if (!head)
		return (NULL);
	last = head;
	while (last->next)
		last = last->next;
	parser_skip_whitespace(s);
	while (**s == '&' && *(*s + 1) == '&')
		pt_attach_and(s, mshell, &last);
	return (head);
}

t_ent	*parse_list(const char **s, t_mshell *mshell)
{
	t_ent	*head;
	t_ent	*last;

	head = parse_and(s, mshell);
	if (!head)
		return (NULL);
	last = head;
	while (last->next)
		last = last->next;
	parser_skip_whitespace(s);
	while (**s == '|' && *(*s + 1) == '|')
		pt_attach_or(s, mshell, &last);
	return (head);
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
