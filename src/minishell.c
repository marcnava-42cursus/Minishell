/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:46:11 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/30 20:45:29 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_config	config;
	t_envp		*envp;
	t_ent		*tree;
	int			i;

	if (argc != 1)
		printf("Usage: %s\n", argv[0]);
	if (!env || !*env)
		printf("Error: Can't load env variables\n");
	printf("Loading config\n");
	load_config(&config, env);
	printf("Config loaded\n");
	printf("Saving envp\n");
	envp = save_envp(env);
	if (!envp)
		return (printf("Error saving envp\n"), 1);
	printf("Envp saved\n");
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
			tree = parse_simple_command(line);
			if (!tree)
				printf("Error parsing command\n");
			else
			{
				i = 0;
				while (tree->argv && tree->argv[i])
				{
					printf("argv[%d] = %s\n", i, tree->argv[i]);
					i++;
				}
				ent_free(tree);
			}
		}
		free(line);
	}
	return (0);
}
