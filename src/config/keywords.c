/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keywords.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:54 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:25:50 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*get_gitbranch(void)
{
	int		fd;
	char	buffer[256];
	char	*ref;

	fd = open(".git/HEAD", O_RDONLY);
	if (fd < 0)
		return ("");
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	ref = ft_strnstr(buffer, "refs/heads/", ft_strlen(buffer));
	if (ref)
		return (ref + 11);
	return ("");
}
