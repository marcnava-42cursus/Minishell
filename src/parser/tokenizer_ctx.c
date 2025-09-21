/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_ctx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:55:20 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:55:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	tkn_init_ctx(t_tokctx *c, const char *s)
{
	c->cap = ft_strlen(s) + 1;
	c->buf = ft_calloc(c->cap, sizeof(char));
	c->out = 0;
	c->p = s;
	c->in_squote = 0;
	c->in_dquote = 0;
}

void	tkn_push(t_tokctx *c, char ch)
{
	c->buf[c->out++] = ch;
}
