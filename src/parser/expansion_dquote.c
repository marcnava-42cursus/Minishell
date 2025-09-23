/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dquote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:09:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:09:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	expdq_dollar(const char **p, char **out, t_envp *envp, int exit_code)
{
	/* Use the common dollar expansion logic */
	exp_dollar(p, out, envp, exit_code);
}

void	expand_in_dquotes(const char **p, char **out,
	t_envp *envp, int exit_code)
{
	/* Preservar siempre las comillas dobles alrededor del contenido expandido */
	(*p)++;
	append_char(out, '"');
	while (**p && **p != '"')
	{
		if (**p == '$' && (ft_isalpha(*((*p) + 1)) || *((*p) + 1) == '_'
				|| *((*p) + 1) == '?'))
		{
			expdq_dollar(p, out, envp, exit_code);
			continue ;
		}
		append_char(out, *(*p)++);
	}
	if (**p == '"')
		(*p)++;
	append_char(out, '"');
}
