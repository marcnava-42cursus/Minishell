/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:17 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/05 18:57:17 by marcnava         ###   ########.fr       */
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

int	parse_command(t_envp *envp, t_config *config, t_ent **tree, char *cmd)
{
	char	*exp;
	t_ent	*root;

	if (!envp || !cmd || !tree || !config)
		return (1);
	exp = expand_variables(cmd, envp, config->exit_code);
	if (!exp)
		return (write(2, "Error allocating\n", 17), 1);
	
	/* DEBUG: Print original and expanded */
	// printf("DEBUG: Original: [%s]\n", cmd);
	// printf("DEBUG: Expanded: [%s]\n", exp);
	
	/* Handle empty command after expansion */
	if (is_empty_or_whitespace(exp))
	{
		ft_free((void **)&exp);
		*tree = NULL;
		return (0); /* Not an error, just no command to execute */
	}
	
	root = parse_command_tree(exp);
	if (!root)
	{
		ft_free((void **)&exp);
		return (write(2, "Parse error\n", 12), 1);
	}
	*tree = root;
	ft_free((void **)&exp);
	return (0);
}
