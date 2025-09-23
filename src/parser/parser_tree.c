/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 19:35:16 by marcnava         ###   ########.fr       */
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
	advance_to_last(&last);
	parser_skip_whitespace(s);
	return (handle_pipe_parsing(s, mshell, head, last));
}

t_ent	*parse_and(const char **s, t_mshell *mshell)
{
	t_ent	*left;

	left = parse_pipeline(s, mshell);
	if (!left)
		return (NULL);
	parser_skip_whitespace(s);
	return (handle_and_parsing(s, mshell, left));
}

t_ent	*parse_list(const char **s, t_mshell *mshell)
{
	t_ent	*left;

	left = parse_and(s, mshell);
	if (!left)
		return (NULL);
	parser_skip_whitespace(s);
	return (handle_or_parsing(s, mshell, left));
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
