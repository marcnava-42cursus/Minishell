/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:47 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/05 18:57:48 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char *parse_var(const char **s)
{
	const char *start;
	size_t len;

	start = *s + 1;
	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	*s = start + len;
	return (ft_substr(start, 0, len));
}

char *expand_variables(const char *in, t_envp *envp, int exit_code)
{
	char *out = ft_strdup("");
	char *var, *val, *tmp, *seg, *num;
	const char *start;

	while (*in)
	{
		if (ft_strncmp(in, "$?", 2) == 0)
		{
			num = ft_itoa(exit_code);
			tmp = ft_strjoin(out, num);
			ft_free((void **)&out);
			ft_free((void **)&num);
			out = tmp;
			in += 2;
		}
		else if (*in == '$' && (ft_isalpha(*(in + 1)) || *(in + 1) == '_'))
		{
			var = parse_var(&in);
			if (var && *var)
			{
				val = get_env_value(envp, var);
				if (!val)
					val = "";
				tmp = ft_strjoin(out, val);
				ft_free((void **)&out);
				out = tmp;
			}
			else
			{
				/* Handle invalid variable name - treat as literal $ */
				tmp = ft_strjoin(out, "$");
				ft_free((void **)&out);
				out = tmp;
				in++;
			}
			if (var)
				ft_free((void **)&var);
		}
		else if (*in == '\'' || *in == '\"')
		{
			char quote = *in++;
			start = in;
			while (*in && *in != quote)
				in++;
			seg = ft_substr(start - 1, 0, in - start + 2);
			tmp = ft_strjoin(out, seg);
			ft_free((void **)&out);
			ft_free((void **)&seg);
			out = tmp;
			if (*in == quote)
				in++;
		}
		else
		{
			start = in;
			while (*in && *in != '$' && *in != '\"' && *in != '\'')
				in++;
			seg = ft_substr(start, 0, in - start);
			tmp = ft_strjoin(out, seg);
			ft_free((void **)&out);
			ft_free((void **)&seg);
			out = tmp;
		}
	}
	return (out);
}
