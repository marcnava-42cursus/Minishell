/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:08:03 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/02 12:18:59 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_command(t_envp *envp, t_ent **tree, char *cmd)
{
	char	*exp;
	t_ent	*root;

	if (!envp || !cmd || !tree)
		return (1);
	exp = expand_variables(cmd, envp);
	if (!exp)
		return (write(2, "Error allocating\n", 17), 1);
	root = parse_command_tree(exp);
	if (!root)
	{
		ft_free((void **)&exp);
		return (write(2, "Parse error\n", 12), 1);
	}
	print_tree(root, 0);
	ent_free(root);
	ft_free((void **)&exp);
	*tree = NULL;
	return (0);
}
