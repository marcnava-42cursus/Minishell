/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main_run.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:06:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:06:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	pm_prepare(t_mshell *mshell, const char *cmd, char **out_exp)
{
	char	*exp;

	if (!pm_store_raw_command(mshell, cmd))
		return (-1);
	exp = pm_expand_cmd(cmd, mshell);
	if (!exp)
		return (-1);
	if (pm_is_empty_or_whitespace(exp))
	{
		ft_free((void **)&exp);
		mshell->tree = NULL;
		return (0);
	}
	if (!pm_store_expanded(mshell, exp))
		return (ft_free((void **)&exp), -1);
	*out_exp = exp;
	return (1);
}

int	pm_parse_store(t_mshell *mshell, char *exp)
{
	t_ent	*root;

	root = pm_parse_tree_or_error(exp, mshell);
	if (!root)
	{
		ft_free((void **)&exp);
		mshell->tree = NULL;
		return (0);
	}
	mshell->tree = root;
	ft_free((void **)&exp);
	return (1);
}

int	pm_run(t_mshell *mshell, const char *cmd)
{
	char	*exp;
	int		st;

	st = pm_prepare(mshell, cmd, &exp);
	if (st <= 0)
		return (0);
	pm_parse_store(mshell, exp);
	return (0);
}
