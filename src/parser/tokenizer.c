/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:20:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 00:20:30 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_next_token(const char **s)
{
	char	*op;

	parser_skip_whitespace(s);
	if (**s == '\0')
		return (NULL);
	op = tkn_return_operator(s);
	if (op)
		return (op);
	return (tkn_read_word(s));
}
