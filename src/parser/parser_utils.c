/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/02 14:10:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* forward declarations */
static void   skip_whitespace(const char **s);
static char   *get_next_token(const char **s);
static char   **realloc_argv(char **old, int oldc, char *tok);
static t_ent  *parse_cmd(const char **s);
static t_ent  *parse_subshell(const char **s);
static t_ent  *parse_primary(const char **s);
static t_ent  *parse_pipeline(const char **s);
static t_ent  *parse_and(const char **s);
static t_ent  *parse_list(const char **s);

static void skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

static char *get_next_token(const char **s)
{
	const char *start;
	size_t      len;

	skip_whitespace(s);
	if (**s == '\0') return (NULL);
	if (**s == '(' || **s == ')')
		return (ft_substr((*s)++, 0, 1));
	if (**s == '&' && *(*s + 1) == '&')
		return ((*s) += 2, ft_strdup("&&"));
	if (**s == '|' && *(*s + 1) == '|')
		return ((*s) += 2, ft_strdup("||"));
	if (**s == '|')
		return ((*s)++, ft_strdup("|"));
	if (**s == '"') {
		start = *s; (*s)++;
		while (**s && **s != '"') (*s)++;
		if (**s == '"') (*s)++;
		return (ft_substr(start, 0, (size_t)(*s - start)));
	}
	start = *s; len = 0;
	while ((*s)[len] && !ft_strchr(" \t()&|", (*s)[len])) len++;
	*s += len;
	return (ft_substr(start, 0, len));
}

static char **realloc_argv(char **old, int oldc, char *tok)
{
	char **new;
	int   i;

	new = ft_calloc(oldc + 2, sizeof(char *));
	if (!new)
	{
		ft_free((void **)&tok);
		return (old);
	}
	i = 0;
	while (i < oldc)
	{
		new[i] = old[i];
		i++;
	}
	new[i] = tok;
	ft_free((void **)&old);
	return (new);
}

static t_ent *parse_cmd(const char **s)
{
	char    *tok;
	char   **argv;
	int     argc;

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
		/* SIEMPRE parar en |, &, o ')' */
		if (!**s || **s == '|' || **s == '&' || **s == ')')
			break ;
		tok = get_next_token(s);
		if (!tok)
			break ;
		argv = realloc_argv(argv, argc, tok);
		argc++;
	}
	argv[argc] = NULL;
	return (ent_new_node(NODE_COMMAND, argv));
}

static t_ent *parse_subshell(const char **s)
{
	const char *start;
	const char *p;
	int         depth;
	char       *content;
	char      **argv;
	t_ent      *child;
	t_ent      *node;

	(*s)++;                         /* skip '(' */
	start = *s;
	depth = 1;
	p = start;
	while (*p && depth)
	{
		if (*p == '(') depth++;
		else if (*p == ')') depth--;
		p++;
	}
	content = ft_substr(start, 0, (size_t)(p - start - 1));
	if (!content) return (NULL);
	child = parse_list(s);
	if (**s == ')') (*s)++;
	argv = ft_calloc(2, sizeof(char *));
	if (!argv) return (ft_free((void **)&content), NULL);
	argv[0] = content;
	node = ent_new_node(NODE_SUBSHELL, argv);
	if (node) node->child = child;
	return (node);
}

static t_ent *parse_primary(const char **s)
{
	skip_whitespace(s);
	if (**s == '(')
		return (parse_subshell(s));
	return (parse_cmd(s));
}

static t_ent *parse_pipeline(const char **s)
{
	t_ent *head;
	t_ent *last;
	t_ent *op;
	t_ent *right;

	head = parse_primary(s);
	if (!head) return (NULL);
	last = head;
	while (last->next) last = last->next;
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
		while (last->next) last = last->next;
		skip_whitespace(s);
	}
	return (head);
}

static t_ent *parse_and(const char **s)
{
	t_ent *head;
	t_ent *last;
	t_ent *op;
	t_ent *sub;

	head = parse_pipeline(s);
	if (!head) return (NULL);
	last = head;
	while (last->next) last = last->next;
	skip_whitespace(s);
	while (**s == '&' && *(*s + 1) == '&')
	{
		(*s) += 2;
		op = ent_new_node(NODE_AND, NULL);
		last->next = op;
		last = op;
		sub = parse_pipeline(s);
		last->next = sub;
		while (last->next) last = last->next;
		skip_whitespace(s);
	}
	return (head);
}

static t_ent *parse_list(const char **s)
{
	t_ent *head;
	t_ent *last;
	t_ent *op;
	t_ent *sub;

	head = parse_and(s);
	if (!head) return (NULL);
	last = head;
	while (last->next) last = last->next;
	skip_whitespace(s);
	while (**s == '|' && *(*s + 1) == '|')
	{
		(*s) += 2;
		op = ent_new_node(NODE_OR, NULL);
		last->next = op;
		last = op;
		sub = parse_and(s);
		last->next = sub;
		while (last->next) last = last->next;
		skip_whitespace(s);
	}
	return (head);
}

t_ent *parse_command_tree(const char *cmd)
{
	const char *p = cmd;
	return (parse_list(&p));
}
