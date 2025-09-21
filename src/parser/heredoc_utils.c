/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:58:20 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:58:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_heredoc_quotes(const char *cmd, const char *delimiter_pos)
{
	const char	*search_pos;

	(void)delimiter_pos;
	if (!cmd)
		return (-1);
	search_pos = ft_strnstr(cmd, "<<", ft_strlen(cmd));
	if (!search_pos)
		return (-1);
	search_pos += 2;
	while (*search_pos == ' ' || *search_pos == '\t')
		search_pos++;
	if (*search_pos == '\'' || *search_pos == '\"')
		return (1);
	return (0);
}

const char	*hd_after_arrow(const char *cmd)
{
	const char	*pos;

	if (!cmd)
		return (NULL);
	pos = ft_strnstr(cmd, "<<", ft_strlen(cmd));
	if (!pos)
		return (NULL);
	pos += 2;
	while (*pos == ' ' || *pos == '\t')
		pos++;
	return (pos);
}

char	*hd_extract_delim(const char *pos)
{
	const char	*end;
	char		quote;

	if (!pos)
		return (NULL);
	if (*pos == '\'' || *pos == '\"')
	{
		quote = *pos;
		end = pos + 1;
		while (*end && *end != quote)
			end++;
		if (*end == quote)
			end++;
		return (ft_substr(pos, 0, (size_t)(end - pos)));
	}
	end = pos;
	while (*end && *end != ' ' && *end != '\t' && *end != '\n'
		&& *end != '|' && *end != '&')
		end++;
	return (ft_substr(pos, 0, (size_t)(end - pos)));
}

char	*hd_clean_delimiter(const char *delimiter, int *is_quoted)
{
	size_t	len;
	char	quote;

	*is_quoted = 0;
	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len >= 2 && (delimiter[0] == '\'' || delimiter[0] == '\"'))
	{
		quote = delimiter[0];
		if (delimiter[len - 1] == quote)
		{
			*is_quoted = 1;
			return (ft_substr(delimiter, 1, len - 2));
		}
	}
	return (ft_strdup(delimiter));
}
