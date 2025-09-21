/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:55:40 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:55:40 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	tkn_handle_backslash(t_tokctx *c)
{
	if (c->in_squote)
		return (tkn_push(c, *c->p++));
	if (c->in_dquote)
	{
		if (*(c->p + 1) == '\n')
			c->p += 2;
		else
		{
			tkn_push(c, *c->p++);
			if (*c->p)
				tkn_push(c, *c->p++);
		}
		return ;
	}
	if (*(c->p + 1) == '\n')
		c->p += 2;
	else
	{
		tkn_push(c, *c->p++);
		if (*c->p)
			tkn_push(c, *c->p++);
	}
}

void	tkn_loop(t_tokctx *c)
{
	while (*c->p)
	{
		if (!c->in_squote && !c->in_dquote && (*c->p == ' ' || *c->p == '\t'
				|| *c->p == '(' || *c->p == ')' || *c->p == '&' || *c->p == '|'
				|| *c->p == '<' || *c->p == '>'))
			break ;
		if (*c->p == '\\')
			tkn_handle_backslash(c);
		else if (*c->p == '\'' && !c->in_dquote)
		{
			c->in_squote = !c->in_squote;
			tkn_push(c, *c->p++);
		}
		else if (*c->p == '"' && !c->in_squote)
		{
			c->in_dquote = !c->in_dquote;
			tkn_push(c, *c->p++);
		}
		else
			tkn_push(c, *c->p++);
	}
}

char	*tkn_read_word(const char **s)
{
	t_tokctx	c;

	tkn_init_ctx(&c, *s);
	tkn_loop(&c);
	c.buf[c.out] = '\0';
	*s = c.p;
	return (c.buf);
}
