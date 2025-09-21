/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_ops.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:55:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:55:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*tkn_return_operator(const char **s)
{
	if (**s == '(' || **s == ')')
		return (ft_substr((*s)++, 0, 1));
	if (**s == '&' && *(*s + 1) == '&')
		return ((*s) += 2, ft_strdup("&&"));
	if (**s == '|' && *(*s + 1) == '|')
		return ((*s) += 2, ft_strdup("||"));
	if (**s == '<' && *(*s + 1) == '<')
		return ((*s) += 2, ft_strdup("<<"));
	if (**s == '>' && *(*s + 1) == '>')
		return ((*s) += 2, ft_strdup(">>"));
	if (**s == '<')
		return ((*s)++, ft_strdup("<"));
	if (**s == '>')
		return ((*s)++, ft_strdup(">"));
	if (**s == '|')
		return ((*s)++, ft_strdup("|"));
	return (NULL);
}
