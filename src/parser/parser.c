/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:08:03 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/01 18:46:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	count_subshells(const char *cmd)
{
	int	depth;
	int	count;
	int	i;

	depth = 0;
	count = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '(')
		{
			depth++;
			if (depth == 1)
				count++;
		}
		else if (cmd[i] == ')' && depth > 0)
			depth--;
		i++;
	}
	return (count);
}

static char **split_subshell(const char *cmd)
{
	int		total;
	char	**subs;
	int		depth;
	int		start;
	int		i;
	int		idx;

	total = count_subshells(cmd);
	subs = ft_calloc(total + 1, sizeof(char *));
	if (!subs)
		return (NULL);
	depth = 0;
	start = -1;
	i = 0;
	idx = 0;
	while (cmd[i])
	{
		if (cmd[i] == '(')
		{
			depth++;
			if (depth == 1)
				start = i;
		}
		else if (cmd[i] == ')')
		{
			if (depth == 1 && start >= 0)
				subs[idx++] = ft_substr(cmd,
					start + 1, i - start - 1);
			if (depth > 0)
				depth--;
		}
		i++;
	}
	subs[idx] = NULL;
	return (subs);
}

static int	check_openclose(const char *cmd)
{
	int	parenthesis;
	int	quote;
	int	squote;
	int	i;

	parenthesis = 0;
	quote = 0;
	squote = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '(')
			parenthesis++;
		else if (cmd[i] == '\'')
			squote++;
		else if (cmd[i] == '"')
			quote++;
		else if (cmd[i] == ')')
			parenthesis--;
		i++;
		if (parenthesis < 0)
		{
			printf("Quotes: %d\nSingle Quotes: %d\nParenthesis: %d\n", quote, squote, parenthesis);
			return (1);
		}
	}
	if (parenthesis != 0 || (quote != 0 && quote % 2 != 0) || (squote != 0 && squote % 2 != 0))
	{
		printf("Quotes: %d\nSingle Quotes: %d\nParenthesis: %d\n", quote, squote, parenthesis);
		return (1);
	}
	return (0);
}

int	parse_command(t_envp *envp, t_ent **tree, char *cmd)
{
	char	*expanded;

	if (!envp || !cmd || !tree)
		return (1);
	expanded = expand_variables(cmd, envp);
	if (!expanded)
		return (write(2, "Error allocating\n", 17), 1);
	printf("%s\n", expanded);
	free(expanded);
	return (0);
}

