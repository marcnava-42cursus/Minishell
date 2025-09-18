/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:03:30 by agentmode         #+#    #+#             */
/*   Updated: 2025/09/05 23:11:58 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

char	*wc_join_path(const char *dir, const char *name)
{
	char	*pre;
	char	*out;
	size_t	len;

	if (!dir || ft_strcmp((char *)dir, ".") == 0)
		return (ft_strdup(name));
	len = ft_strlen(dir);
	if (len && dir[len - 1] == '/')
		pre = ft_strdup(dir);
	else
		pre = ft_strjoin(dir, "/");
	if (!pre)
		return (NULL);
	out = ft_strjoin(pre, name);
	ft_free((void **)&pre);
	return (out);
}

char	**wc_expand_in_dir(const char *base, const char *segment, int *outc)
{
	DIR				*d;
	struct dirent	*de;
	char			**out;
	char			*path;

	*outc = 0;
	out = NULL;
	if (base && *base)
		d = opendir(base);
	else
		d = opendir(".");
	if (!d)
		return (NULL);
	while ((de = readdir(d)))
	{
		if (!ft_strcmp(de->d_name, ".") || !ft_strcmp(de->d_name, ".."))
			continue ;
		if (!wc_match_segment(de->d_name, segment))
			continue ;
		if (base && *base)
			path = wc_join_path(base, de->d_name);
		else
			path = wc_join_path(".", de->d_name);
		if (!path)
			continue ;
		out = ft_realloc_matrix(out, *outc, path);
		*outc += 1;
	}
	closedir(d);
	return (out);
}

char	**wc_next_paths(char **bases, int basec, const char *segment, int *outc)
{
	int		c;
	int		i;
	char	**res;
	char	**tmp;
	int		j;

	*outc = 0;
	res = NULL;
	i = 0;
	while (i < basec)
	{
		tmp = wc_expand_in_dir(bases[i], segment, &c);
		j = 0;
		while (tmp && j < c)
		{
			res = ft_realloc_matrix(res, *outc, tmp[j]);
			*outc += 1;
			j++;
		}
		ft_free((void **)&tmp);
		i++;
	}
	return (res);
}
