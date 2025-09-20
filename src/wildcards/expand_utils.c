/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 19:59:21 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

static char	**init_bases(const char *pattern, int *count)
{
	char	**out;

	out = ft_calloc(2, sizeof(char *));
	if (!out)
		return (NULL);
	if (pattern && pattern[0] == '/')
		out[0] = ft_strdup("/");
	else
		out[0] = ft_strdup(".");
	if (!out[0])
		return (ft_free((void **)&out), NULL);
	out[1] = NULL;
	*count = 1;
	return (out);
}

char	**wc_split_or_empty(const char *pattern)
{
	if (pattern)
		return (ft_split(pattern, '/'));
	return (ft_split("", '/'));
}

int	wc_init_walk(const char *pattern, t_walk *w, char ***segs)
{
	*w = (t_walk){0};
	*segs = wc_split_or_empty(pattern);
	if (!*segs)
		return (0);
	w->bases = (char **)0;
	w->bases = (char **)init_bases(pattern, &w->basec);
	if (!w->bases)
		return (ft_free_matrix((void **)*segs), 0);
	w->i = 0;
	return (1);
}
