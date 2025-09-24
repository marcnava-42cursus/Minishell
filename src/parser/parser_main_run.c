/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main_run.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:06:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/24 22:19:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	pm_is_escaped(const char *str, size_t pos)
{
	size_t	count;

	if (pos == 0)
		return (0);
	count = 0;
	while (pos-- > 0 && str[pos] == '\\')
		count++;
	return (count % 2 == 1);
}

static int	pm_has_unclosed_quotes(const char *cmd)
{
	size_t	i;
	int		in_squote;
	int		in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (cmd[i])
	{
		if (!in_squote && cmd[i] == '\\')
		{
			if (cmd[i + 1])
				i += 2;
			else
				i++;
			continue ;
		}
		if (!in_dquote && cmd[i] == '\'' && !pm_is_escaped(cmd, i))
			in_squote = !in_squote;
		else if (!in_squote && cmd[i] == '"' && !pm_is_escaped(cmd, i))
			in_dquote = !in_dquote;
		i++;
	}
	return (in_squote || in_dquote);
}

static int	pm_validate_quotes(const char *cmd, t_mshell *mshell)
{
	if (!cmd)
		return (1);
	if (!pm_has_unclosed_quotes(cmd))
		return (1);
	print_err2("minishell: syntax error: unexpected end of file\n", NULL, NULL);
	if (mshell->exit_code == 0)
		mshell->exit_code = 2;
	mshell->tree = NULL;
	return (0);
}

int	pm_prepare(t_mshell *mshell, const char *cmd, char **out_exp)
{
	char	*exp;

	if (!pm_store_raw_command(mshell, cmd))
		return (-1);
	if (!pm_validate_quotes(cmd, mshell))
		return (0);
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
