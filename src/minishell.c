/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:46:11 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/29 18:44:16 by marcnava         ###   ########.fr       */
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
			tree = parse_simple_command(line);
			if (tree)
			{
				config.exit_code = exec(tree, envp);
				if (config.exit_code != 0)
					printf("Command exited with code: %d\n", config.exit_code);
				ent_free(tree);
				if (config.prompt)
					free(config.prompt);
				config.prompt=build_prompt(config.prompt_raw,config.exit_code);
			}
			else
				printf("Error parsing command\n");
		}
		free(line);
	}
	return (0);
}
