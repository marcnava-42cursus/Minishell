/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava  <marcnava @warp.ai>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/05 21:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_H
# define WILDCARDS_H

# include <dirent.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"
# include "utils.h"

/* Internal contexts */
typedef struct s_walk
{
	char	**bases;
	int		basec;
	int		i;
	int		c;
}t_walk;

typedef struct s_adv
{
	char		**bases;
	int			basec;
	const char	*seg;
	int			is_last;
}t_adv;

typedef struct s_dirctx
{
	char		***out;
	int			*outc;
	const char	*base;
	const char	*segment;
}t_dirctx;

struct s_mw
{
	int			i;
	int			j;
	int			star;
	int			mark;
	const char	*s;
	const char	*p;
};

/* Pattern helpers */
int	wc_contains_wild(const char *s);
int	wc_is_quoted_token(const char *s);
int	wc_match_segment(const char *name, const char *pattern);

/* Path helpers */
char	*wc_join_path(const char *dir, const char *name);
char	**wc_next_paths(char **bases, int basec, const char *segment, int *outc);

/* Expansion entry point */
char	**wc_expand(const char *pattern, int *outc);
int	wc_token_should_expand(const char *token);

/* Expansion utils */
char	**wc_split_or_empty(const char *pattern);
int	wc_init_walk(const char *pattern, t_walk *w, char ***segs);

/* Utils */
void	wc_sort_strings(char **arr, int n);
void	wc_free_matrix(char **arr);
int	wc_path_is_dir(const char *path);
int	wc_path_exists(const char *path);

#endif
