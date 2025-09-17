/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:20:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:26:13 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*safe_strjoin_free(char *dst, const char *src)
{
	char	*tmp;

	if (!dst || !src)
		return (dst);
	tmp = ft_strjoin(dst, src);
	ft_free((void **)&dst);
	return (tmp);
}

int	safe_append_str(char **dst, const char *src)
{
	char	*newbuf;

	if (!dst || !src)
		return (0);
	newbuf = safe_strjoin_free(*dst, src);
	if (!newbuf)
		return (0);
	*dst = newbuf;
	return (1);
}

int	write_config_lines(int fd, const char **lines)
{
	int	i;

	if (fd < 0 || !lines)
		return (1);
	i = 0;
	while (lines[i])
	{
		if (write(fd, lines[i], ft_strlen(lines[i])) < 0)
			return (1);
		i++;
	}
	return (0);
}
