/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dquote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:09:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 19:41:51 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	expand_in_dquotes(const char **p, char **out,
	t_envp *envp, int exit_code)
{
	(*p)++;
	append_char(out, '"');
	while (**p && **p != '"')
	{
		if (**p == '$' && (ft_isalpha(*((*p) + 1)) || *((*p) + 1) == '_'
				|| *((*p) + 1) == '?'))
		{
			exp_dollar(p, out, envp, exit_code);
			continue ;
		}
		append_char(out, *(*p)++);
	}
	if (**p == '"')
		(*p)++;
	append_char(out, '"');
}
