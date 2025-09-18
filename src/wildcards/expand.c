/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:06:00 by agentmode         #+#    #+#             */
/*   Updated: 2025/09/05 23:19:18 by marcnava         ###   ########.fr       */
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

static char	**advance_literal(char **bases, int basec, const char *seg,
	int is_last, int *outc)
{
	int			i;
	char		*joined;
	char		**res;

	*outc = 0;
	res = NULL;
	i = 0;
	while (i < basec)
	{
		joined = wc_join_path(bases[i], seg);
		if (joined && ((is_last && wc_path_exists(joined))
				|| (!is_last && wc_path_is_dir(joined))))
		{
			res = ft_realloc_matrix(res, *outc, joined);
			*outc += 1;
		}
		else if (joined)
			ft_free((void **)&joined);
		i++;
	}
	return (res);
}

int	wc_token_should_expand(const char *token)
{
	if (!token)
		return (0);
	if (!wc_contains_wild(token))
		return (0);
	return (!wc_is_quoted_token(token));
}

char	**wc_expand(const char *pattern, int *outc)
{
	char	**segs;
	char	**bases;
	char	**next;
	int		basec;
	int		i;
	int		c;

	*outc = 0;
	if (pattern)
		segs = ft_split(pattern, '/');
	else
		segs = ft_split("", '/');
	if (!segs)
		return (NULL);
	bases = init_bases(pattern, &basec);
	if (!bases)
		return (ft_free_matrix((void **)segs), NULL);
	i = 0;
	while (segs[i])
	{
		if (wc_contains_wild(segs[i]))
			next = wc_next_paths(bases, basec, segs[i], &c);
		else
			next = advance_literal(bases, basec, segs[i],
					segs[i + 1] == NULL, &c);
		wc_free_matrix(bases);
		bases = next;
		basec = c;
		if (!bases || basec == 0)
			break ;
		i++;
	}
	ft_free_matrix((void **)segs);
	if (!bases || basec == 0)
		return (NULL);
	wc_sort_strings(bases, basec);
	*outc = basec;
	return (bases);
}
