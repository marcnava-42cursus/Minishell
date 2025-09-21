/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:48:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:01:36 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

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
