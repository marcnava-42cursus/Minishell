/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:08:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:08:30 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*read_var(const char **p)
{
	const char	*start;
	size_t		len;

	start = *p + 1;
	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	*p = start + len;
	return (ft_substr(start, 0, len));
}

void	append_str(char **dst, const char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst, src);
	ft_free((void **)dst);
	*dst = tmp;
}

void	append_char(char **dst, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	append_str(dst, buf);
}
