/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:02:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:01:11 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

static int	mw_step(t_mw *st)
{
	if (st->p[st->j] == '*')
		return (st->star = st->j, st->j++, st->mark = st->i, 1);
	else if (st->p[st->j] == st->s[st->i])
		return (st->i++, st->j++, 1);
	else if (st->star != -1)
		return (st->j = st->star + 1, st->i = ++st->mark, 1);
	return (0);
}

static int	match_wild(const char *s, const char *p)
{
	t_mw	st;

	st = (t_mw){0};
	st.s = s;
	st.p = p;
	st.star = -1;
	st.mark = 0;
	while (st.s[st.i])
		if (!mw_step(&st))
			return (0);
	while (st.p[st.j] == '*')
		st.j++;
	return (st.p[st.j] == '\0');
}

int	wc_match_segment(const char *name, const char *pattern)
{
	if (!name || !pattern)
		return (0);
	if (name[0] == '.' && pattern[0] != '.')
		return (0);
	return (match_wild(name, pattern));
}
