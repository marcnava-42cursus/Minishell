/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:21:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:33:17 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	pt_advance_to_last(t_ent **last)
{
	while ((*last)->next)
		*last = (*last)->next;
}

void	pt_attach_pipe(const char **s, t_mshell *mshell, t_ent **last)
{
	(*s)++;
	(*last)->next = ent_new_node(NODE_PIPE, NULL);
	*last = (*last)->next;
	parser_skip_whitespace(s);
	(*last)->next = parse_primary(s, mshell);
	pt_advance_to_last(last);
	parser_skip_whitespace(s);
}

void	pt_attach_and(const char **s, t_mshell *mshell, t_ent **last)
{
	(*s) += 2;
	(*last)->next = ent_new_node(NODE_AND, NULL);
	*last = (*last)->next;
	(*last)->next = parse_pipeline(s, mshell);
	pt_advance_to_last(last);
	parser_skip_whitespace(s);
}

void	pt_attach_or(const char **s, t_mshell *mshell, t_ent **last)
{
	(*s) += 2;
	(*last)->next = ent_new_node(NODE_OR, NULL);
	*last = (*last)->next;
	(*last)->next = parse_and(s, mshell);
	pt_advance_to_last(last);
	parser_skip_whitespace(s);
}
