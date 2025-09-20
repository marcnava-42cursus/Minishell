/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_numparse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:27:54 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:50:10 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

static int	parse_sign(const char *s, int *i)
{
	int	sign;

	sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			sign = -1;
		else
			sign = 1;
		*i += 1;
	}
	return (sign);
}

static t_ull	limit_for_sign(int sign)
{
	if (sign < 0)
		return ((t_ull)LLONG_MAX + 1ULL);
	return ((t_ull)LLONG_MAX);
}

static int	accumulate_digits(const char *s, int *i, t_ull limit, t_ull *acc)
{
	int	d;

	while (s[*i])
	{
		if (!ft_isdigit(s[*i]))
			return (0);
		d = s[*i] - '0';
		if (*acc > (limit / 10ULL)
			|| (*acc == (limit / 10ULL) && (t_ull)d > (limit % 10ULL)))
			return (0);
		*acc = (*acc) * 10ULL + (t_ull)d;
		*i += 1;
	}
	return (1);
}

static void	assign_result(int sign, t_ull acc, long long *out)
{
	if (sign > 0)
		*out = (long long)acc;
	else if (acc == (t_ull)LLONG_MAX + 1ULL)
		*out = LLONG_MIN;
	else
		*out = -(long long)acc;
}

int	parse_ll_strict(const char *s, long long *out)
{
	int		i;
	int		sign;
	t_ull		acc;
	t_ull		limit;

	if (!s || !*s)
		return (0);
	i = 0;
	sign = parse_sign(s, &i);
	if (!s[i])
		return (0);
	acc = 0ULL;
	limit = limit_for_sign(sign);
	if (!accumulate_digits(s, &i, limit, &acc))
		return (0);
	assign_result(sign, acc, out);
	return (1);
}
