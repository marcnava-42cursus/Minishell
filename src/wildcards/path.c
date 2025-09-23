/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:03:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 20:06:10 by marcnava         ###   ########.fr       */
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

static void	append_if_match(t_dirctx *ctx, const char *name)
{
	char	*path;

	if (!ft_strcmp((char *)name, ".") || !ft_strcmp((char *)name, ".."))
		return ;
	if (!wc_match_segment(name, ctx->segment))
		return ;
	if (ctx->base && *ctx->base)
		path = wc_join_path(ctx->base, name);
	else
		path = wc_join_path(".", name);
	if (!path)
		return ;
	*ctx->out = ft_realloc_matrix(*ctx->out, *ctx->outc, path);
	*ctx->outc += 1;
}

char	**wc_expand_in_dir(const char *base, const char *segment, int *outc)
{
	DIR				*d;
	struct dirent	*de;
	char			**out;
	t_dirctx		ctx;

	*outc = 0;
	out = NULL;
	ctx = (t_dirctx){&out, outc, base, segment};
	if (base && *base)
		d = opendir(base);
	else
		d = opendir(".");
	if (!d)
		return (NULL);
	de = readdir(d);
	while (de)
	{
		append_if_match(&ctx, de->d_name);
		de = readdir(d);
	}
	closedir(d);
	return (out);
}

char	**wc_next_paths(char **bases, int basec, const char *segment, int *outc)
{
	int		c;
	int		i;
	int		j;
	char	**res;
	char	**tmp;

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
