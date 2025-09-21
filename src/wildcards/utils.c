/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:04:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:01:11 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

int	wc_path_is_dir(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

int	wc_path_exists(const char *path)
{
	if (!path)
		return (0);
	return (access(path, F_OK) == 0);
}

void	wc_sort_strings(char **arr, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i + 1 < n)
	{
		j = 0;
		while (j + 1 < n - i)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	wc_free_matrix(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		ft_free((void **)&arr[i++]);
	ft_free((void **)&arr);
}
