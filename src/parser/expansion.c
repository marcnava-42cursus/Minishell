/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:23:51 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/02 11:45:14 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* parse_var.c
 * Extracts a variable name starting after '$'.
 * Advances the input pointer beyond the variable name.
 * Returns a newly allocated string with the variable name.
 */
static char	*parse_var(char **s)
{
	char	*start;
	size_t	len;
	char	*var;

	start = *s + 1;
	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	var = ft_substr(start, 0, len);
	*s = start + len;
	return (var);
}

/* append_char.c
 * Append a single character to a dynamically built string.
 * Frees the old string and returns the new one.
 */
static char	*append_char(char *res, char c)
{
	char	tmp[2];
	char	*old;

	tmp[0] = c;
	tmp[1] = '\0';
	old = res;
	res = ft_strjoin(res, tmp);
	ft_free((void **)&old);
	return (res);
}

/* expand_variables.c
 * Iterates input string and builds an output with all shell
 * variables expanded according to single/double quote rules,
 * preserving the quotes in the output:
 *  - In single quotes: copy quotes and content literally.
 *  - In double quotes: copy quotes and content; expand $var.
 *  - Outside quotes: expand $var; copy other chars literally.
 * Returns a newly allocated expanded string.
 */
char	*expand_variables(const char *in, t_envp *envp)
{
	char	*out;
	char	*old;
	char	*var;

	out = ft_strdup(""); /* Start with empty result */
	while (*in)
	{
		if (*in == '\'')
		{
			/* Preserve opening single quote */
			out = append_char(out, *in);
			in++;
			/* Copy until closing single quote */
			while (*in && *in != '\'')
				out = append_char(out, *in++);
			/* Preserve closing single quote */
			if (*in == '\'')
				out = append_char(out, *in++);
		}
		else if (*in == '"')
		{
			/* Preserve opening double quote */
			out = append_char(out, *in);
			in++;
			/* Process until closing double quote */
			while (*in && *in != '"')
			{
				if (*in == '$')
				{
					var = parse_var((char **)&in);
					old = out;
					/* Append variable value */
					out = ft_strjoin(out, get_env_value(envp, var));
					ft_free((void **)&old);
					ft_free((void **)&var);
				}
				else
					out = append_char(out, *in++);
			}
			/* Preserve closing double quote */
			if (*in == '"')
				out = append_char(out, *in++);
		}
		else if (*in == '$')
		{
			var = parse_var((char **)&in);
			old = out;
			/* Append variable value */
			out = ft_strjoin(out, get_env_value(envp, var));
			ft_free((void **)&old);
			ft_free((void **)&var);
		}
		else
			/* Regular character: append literally */
			out = append_char(out, *in++);
		}
	return (out);
}
