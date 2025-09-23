/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:24:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 19:41:26 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	exp_process_char(const char **in, char **out, t_envp *envp, int exit_code)
{
	if (**in == '\'')
	{
		append_char(out, '\'');
		(*in)++;
		while (**in && **in != '\'')
			append_char(out, *(*in)++);
		if (**in == '\'')
		{
			(*in)++;
			append_char(out, '\'');
		}
		return (1);
	}
	if (**in == '"')
	{
		expand_in_dquotes(in, out, envp, exit_code);
		return (1);
	}
	if (**in == '$' && (*(*in + 1) == '?' || ft_isalpha(*(*in + 1))
			|| *(*in + 1) == '_'))
	{
		exp_dollar(in, out, envp, exit_code);
		return (1);
	}
	return (0);
}

char	*exp_expand(const char *in, t_envp *envp, int exit_code)
{
	char	*out;

	out = ft_strdup("");
	while (*in)
	{
		if (exp_process_char(&in, &out, envp, exit_code))
			continue ;
		append_char(&out, *in++);
	}
	return (out);
}
