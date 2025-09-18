/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:04:50 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/19 00:04:50 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	copy_single_quoted(const char *s, size_t *i, char *out, size_t *j)
{
	(*i)++;
	while (s[*i] && s[*i] != '\'')
		out[(*j)++] = s[(*i)++];
	if (s[*i] == '\'')
		(*i)++;
}

static void	copy_double_quoted(const char *s, size_t *i, char *out, size_t *j)
{
	(*i)++;
	while (s[*i] && s[*i] != '"')
		out[(*j)++] = s[(*i)++];
	if (s[*i] == '"')
		(*i)++;
}

static char	*remove_all_quotes(const char *str)
{
	char	*out;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			copy_single_quoted(str, &i, out, &j);
		else if (str[i] == '"')
			copy_double_quoted(str, &i, out, &j);
		else
			out[j++] = str[i++];
	}
	out[j] = '\0';
	return (out);
}

static int	fill_unquoted(char **src, char **dst, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		dst[i] = remove_all_quotes(src[i]);
		if (!dst[i])
			return (1);
		i++;
	}
	return (0);
}

char	**process_argv_quotes(char **argv)
{
	char	**new_argv;
	int		count;

	if (!argv)
		return (NULL);
	count = 0;
	while (argv && argv[count])
		count++;
	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return (NULL);
	if (fill_unquoted(argv, new_argv, count))
		return (ft_free_matrix((void **)new_argv), NULL);
	new_argv[count] = NULL;
	return (new_argv);
}
