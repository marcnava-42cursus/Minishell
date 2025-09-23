/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_copies.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 20:19:56 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 20:20:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

void	copy_squoted(const char *s, size_t *i, char *out, size_t *j)
{
	(*i)++;
	while (s[*i] && s[*i] != '\'')
		out[(*j)++] = s[(*i)++];
	if (s[*i] == '\'')
		(*i)++;
}

void	copy_dquoted(const char *s, size_t *i, char *out, size_t *j)
{
	(*i)++;
	while (s[*i] && s[*i] != '"')
		out[(*j)++] = s[(*i)++];
	if (s[*i] == '"')
		(*i)++;
}
