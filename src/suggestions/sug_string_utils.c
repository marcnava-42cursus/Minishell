/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sug_string_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:32:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 11:32:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"

size_t	sug_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

int	sug_is_prefix(const char *str, const char *prefix)
{
	size_t	i;

	if (!str || !prefix)
		return (0);
	i = 0;
	while (prefix[i])
	{
		if (str[i] != prefix[i])
			return (0);
		i++;
	}
	return (1);
}

char	*sug_strdup(const char *s)
{
	size_t	len;
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	len = sug_strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

void	sug_strcpy(char *dest, const char *src)
{
	size_t	i;

	if (!dest || !src)
		return ;
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void	sug_strcat(char *dest, const char *src)
{
	size_t	dest_len;
	size_t	i;

	if (!dest || !src)
		return ;
	dest_len = sug_strlen(dest);
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
}
