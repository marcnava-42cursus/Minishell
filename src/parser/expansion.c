/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 18:41:23 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/01 18:45:11 by marcnava         ###   ########.fr       */
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

	start = *s + 1; /* Skip the '$' char */
	len = 0;
	/* Count alphanumeric or '_' characters */
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	/* Duplicate the variable name */
	var = ft_substr(start, 0, len);
	/* Move pointer past the variable name */
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
	free(old);
	return (res);
}

/* expand_variables.c
 * Iterates input string and builds an output with all shell
 * variables expanded according to single/double quote rules:
 *  - In single quotes: copy literally (no expansion).
 *  - In double quotes: expand $var, copy others literally.
 *  - Outside quotes: expand $var as well.
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
			/* Skip opening single quote */
			in++;
			/* Copy until closing single quote */
			while (*in && *in != '\'')
				out = append_char(out, *in++);
			/* Skip closing single quote */
			if (*in == '\'')
				in++;
		}
		else if (*in == '"')
		{
			/* Skip opening double quote */
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
					free(old);
					free(var);
				}
				else
					out = append_char(out, *in++);
			}
			/* Skip closing double quote */
			if (*in == '"')
				in++;
		}
		else if (*in == '$')
		{
			var = parse_var((char **)&in);
			old = out;
			out = ft_strjoin(out, get_env_value(envp, var));
			free(old);
			free(var);
		}
		else
			/* Regular character: append to output */
			out = append_char(out, *in++);
		}
	return (out);
}
