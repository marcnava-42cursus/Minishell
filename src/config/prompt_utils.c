/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:25:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:26:35 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

int	parse_keyword_block(const char *raw, int *i, char *kw, char *col)
{
	int	k;
	int	c;

	k = 0;
	while (raw[*i] && !ft_isspace(raw[*i]) && raw[*i] != '}'
		&& k < PROMPT_BUF - 1)
		kw[k++] = raw[(*i)++];
	kw[k] = '\0';
	while (raw[*i] && ft_isspace(raw[*i]))
		(*i)++;
	c = 0;
	if (raw[*i] && raw[*i] != '}')
	{
		while (raw[*i] && raw[*i] != '}' && c < PROMPT_BUF - 1)
			col[c++] = raw[(*i)++];
		col[c] = '\0';
	}
	else
		col[0] = '\0';
	if (raw[*i] == '}')
		return ((*i)++, 1);
	return (0);
}

int	append_single_char(char **res, const char *raw, int *i)
{
	char	tmp[2];

	tmp[0] = raw[*i];
	tmp[1] = '\0';
	if (!safe_append_str(res, tmp))
		return (0);
	(*i)++;
	return (1);
}

int	build_keyword_output(char **res, const char *kw,
	int exit_code, const char *col, t_envp *env)
{
	if (!safe_append_str(res, get_color_code(col)))
		return (0);
	if (!safe_append_str(res, replace_keyword((char *)kw, exit_code, env)))
		return (0);
	if (!safe_append_str(res, reset_color()))
		return (0);
	return (1);
}
