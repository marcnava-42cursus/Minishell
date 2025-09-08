/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:02:00 by agentmode         #+#    #+#             */
/*   Updated: 2025/09/05 23:17:04 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

static int	match_wild(const char *s, const char *p)
{
	int	i;
	int	j;
	int	star;
	int	mark;

	i = 0;
	j = 0;
	star = -1;
	mark = 0;
	while (s[i])
	{
		if (p[j] == '*')
		{
			star = j++;
			mark = i;
		}
		else if (p[j] == s[i])
		{
			i++;
			j++;
		}
		else if (star != -1)
		{
			j = star + 1;
			i = ++mark;
		}
		else
			return (0);
	}
	while (p[j] == '*')
		j++;
	return (p[j] == '\0');
}

int	wc_match_segment(const char *name, const char *pattern)
{
	if (!name || !pattern)
		return (0);
	if (name[0] == '.' && pattern[0] != '.')
		return (0);
	return (match_wild(name, pattern));
}

int	wc_contains_wild(const char *s)
{
	while (s && *s)
		if (*s++ == '*')
			return (1);
	return (0);
}

int	wc_is_quoted_token(const char *s)
{
	while (s && *s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		s++;
	}
	return (0);
}
