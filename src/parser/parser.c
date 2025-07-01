/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:43:15 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/30 20:44:59 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ent	*parse_simple_command(char *input)
{
	t_ent	*cmd;
	char	**argv;

	if (!input)
		return (NULL);
	argv = ft_split(input, ' ');
	if (!argv || !argv[0])
	{
		if (argv)
			ft_free_matrix((void **)argv);
		return (NULL);
	}
	cmd = ent_new_node(NODE_COMMAND, argv);
	if (!cmd)
		ft_free_matrix((void **)argv);
	return (cmd);
}
