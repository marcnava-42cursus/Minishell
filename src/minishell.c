/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:46:11 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/01 13:44:09 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_config	config;
	t_envp		*envp;
	t_ent		*tree;

	if (argc != 1)
		return (printf("Usage: %s\n", argv[0]), 1);
	if (!env || !*env)
		return (printf("Error: Can't load env variables\n"), 1);
	load_config(&config, env);
	envp = save_envp(env);
	if (!envp)
		return (printf("Error saving envp\n"), 1);
	tree = NULL;
	while (1)
	{
		line = readline(config.prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			add_history(line);
			if (parse_command(envp, &tree, line) != 0)
				return (printf("Error parsing command\n"), free(line), 1);
			if (tree)
			{
				print_tree(tree, 0);
				ent_free(tree);
				tree = NULL;
			}
		}
		free(line);
	}
	return (0);
}
