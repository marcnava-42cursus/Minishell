/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/05 19:09:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char **ft_realloc_matrix(char **old, int oldc, char *new_str)
{
	char	**new;
	int		i;

	new = ft_calloc(oldc + 2, sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (i < oldc)
	{
		new[i] = old[i];
		i++;
	}
	new[i++] = new_str;
	new[i] = NULL;
	free(old);
	return (new);
}
