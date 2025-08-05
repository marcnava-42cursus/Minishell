/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:24 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/05 18:57:24 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

static char *get_next_token(const char **s)
{
	const char *start;
	size_t len;

	skip_whitespace(s);
	if (**s == '\0')
		return (NULL);
	if (**s == '(' || **s == ')')
		return (ft_substr((*s)++, 0, 1));
	if (**s == '&' && *(*s + 1) == '&')
		return ((*s) += 2, ft_strdup("&&"));
	if (**s == '|' && *(*s + 1) == '|')
		return ((*s) += 2, ft_strdup("||"));
	if (**s == '|')
		return ((*s)++, ft_strdup("|"));
	if (**s == '"') {
		start = *s;
		(*s)++;
		while (**s && **s != '"')
			(*s)++;
		if (**s == '"')
			(*s)++;
		return (ft_substr(start, 0, (size_t)(*s - start)));
	}
	start = *s;
	len = 0;
	while ((*s)[len] && !ft_strchr(" \t()&|", (*s)[len]))
		len++;
	*s += len;
	return (ft_substr(start, 0, len));
}

t_ent *parse_cmd(const char **s)
{
	char *tok;
	char **argv;
	int argc;

	skip_whitespace(s);
	tok = get_next_token(s);
	if (!tok)
		return (NULL);
	argv = ft_calloc(2, sizeof(char *));
	if (!argv)
		return (ft_free((void **)&tok), NULL);
	argv[0] = tok;
	argc = 1;
	while (1)
	{
		skip_whitespace(s);
		if (!**s || **s == '|' || **s == '&' || **s == ')')
			break ;
		tok = get_next_token(s);
		if (!tok)
			break ;
		argv = ft_realloc_matrix(argv, argc, tok);
		argc++;
	}
	argv[argc] = NULL;
	return (ent_new_node(NODE_COMMAND, argv));
}

t_ent *parse_subshell(const char **s)
{
	const char *start;
	const char *p;
	int depth;
	char *content;
	t_ent *child;
	t_ent *node;
	char **argv;

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
	content = ft_substr(start, 0, (size_t)(p - start - 1));
	if (!content)
		return (NULL);
	child = parse_list(s);
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
