/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:17 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 00:34:08 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int is_empty_or_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}

int	parse_command(t_mshell *mshell, char *cmd)
{
	char	*exp;
	t_ent	*root;

	if (!mshell || !cmd)
		return (1);
	/* Store the raw command */
	if (mshell->raw_command)
		ft_free((void **)&mshell->raw_command);
	mshell->raw_command = ft_strdup(cmd);
	if (!mshell->raw_command)
		return (write(2, "Error allocating\n", 17), 1);
	/* Expand variables */
	exp = expand_variables(cmd, mshell);
	if (!exp)
		return (write(2, "Error allocating\n", 17), 1);
	if (is_empty_or_whitespace(exp))
	{
		ft_free((void **)&exp);
		mshell->tree = NULL;
		return (0);
	}
	/* Store the expanded command */
	if (mshell->expanded_command)
		ft_free((void **)&mshell->expanded_command);
	mshell->expanded_command = ft_strdup(exp);
	/* Parse the command tree */
	root = parse_command_tree(exp, mshell);
	if (!root)
	{
		ft_free((void **)&exp);
		return (write(2, "Parse error\n", 12), 1);
	}
	mshell->tree = root;
	ft_free((void **)&exp);
	return (0);
}
