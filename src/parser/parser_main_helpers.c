/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:24:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 00:24:30 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	pm_store_raw_command(t_mshell *mshell, const char *cmd)
{
	if (mshell->raw_command)
		ft_free((void **)&mshell->raw_command);
	mshell->raw_command = ft_strdup(cmd);
	if (!mshell->raw_command)
	{
		print_err2("minishell: allocation error\n", NULL, NULL);
		mshell->exit_code = 1;
		return (0);
	}
	return (1);
}

char	*pm_expand_cmd(const char *cmd, t_mshell *mshell)
{
	char	*exp;

	exp = exp_expand(cmd, mshell->envp, mshell->exit_code);
	if (!exp)
	{
		print_err2("minishell: allocation error\n", NULL, NULL);
		mshell->exit_code = 1;
		return (NULL);
	}
	return (exp);
}

int	pm_is_empty_or_whitespace(const char *str)
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

int	pm_store_expanded(t_mshell *mshell, const char *exp)
{
	if (mshell->expanded_command)
		ft_free((void **)&mshell->expanded_command);
	mshell->expanded_command = ft_strdup(exp);
	return (mshell->expanded_command != NULL);
}

t_ent	*pm_parse_tree_or_error(const char *exp, t_mshell *mshell)
{
	t_ent      *root;
	const char *p = exp;
    
	root = parse_list(&p, mshell);
	if (!root)
	{
		if (mshell->exit_code == 0)
		{
			print_err2("minishell: syntax error\n", NULL, NULL);
			mshell->exit_code = 2;
		}
		return (NULL);
	}
	return (root);
}
