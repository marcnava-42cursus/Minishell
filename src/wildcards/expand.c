/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:06:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:01:11 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

static char	**advance_literal_ctx(t_adv ctx, int *outc)
{
	int			i;
	char		*joined;
	char		**res;

	*outc = 0;
	res = NULL;
	i = 0;
	while (i < ctx.basec)
	{
		joined = wc_join_path(ctx.bases[i], ctx.seg);
		if (joined && ((ctx.is_last && wc_path_exists(joined))
				|| (!ctx.is_last && wc_path_is_dir(joined))))
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

static int	step_segment(char **segs, t_walk *w)
{
	char	**next;
	t_adv	ctx;

	if (wc_contains_wild(segs[w->i]))
		next = wc_next_paths(w->bases, w->basec, segs[w->i], &w->c);
	else
	{
		ctx = (t_adv){w->bases, w->basec, segs[w->i], segs[w->i + 1] == NULL};
		next = advance_literal_ctx(ctx, &w->c);
	}
	wc_free_matrix(w->bases);
	w->bases = next;
	w->basec = w->c;
	w->i++;
	return (w->bases && w->basec > 0);
}

char	**wc_expand(const char *pattern, int *outc)
{
	char	**segs;
	t_walk	w;

	*outc = 0;
	if (!wc_init_walk(pattern, &w, &segs))
		return (NULL);
	while (segs[w.i])
		if (!step_segment(segs, &w))
			break ;
	ft_free_matrix((void **)segs);
	if (!w.bases || w.basec == 0)
		return (NULL);
	wc_sort_strings(w.bases, w.basec);
	*outc = w.basec;
	return (w.bases);
}
