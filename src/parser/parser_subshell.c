/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:23:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 00:23:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*ps_extract_content(const char **s)
{
	const char	*start;
	const char	*p;
	int			depth;

	(*s)++;
	start = *s;
	depth = 1;
	p = start;
	while (*p && depth)
	{
		if (*p == '(')
			depth++;
		else if (*p == ')')
			depth--;
		p++;
	}
	return (ft_substr(start, 0, (size_t)(p - start - 1)));
}

t_ent	*parse_subshell(const char **s, t_mshell *mshell)
{
	char		*content;
	t_ent		*child;
	t_ent		*node;
	char		**argv;

	content = ps_extract_content(s);
	if (!content)
		return (NULL);
	child = parse_list(s, mshell);
	if (**s == ')')
		(*s)++;
	argv = ft_calloc(2, sizeof(char *));
	if (!argv)
		return (ft_free((void **)&content), NULL);
	argv[0] = content;
	node = ent_new_node(NODE_SUBSHELL, argv);
	if (node)
		node->child = child;
	return (node);
}
