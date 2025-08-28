/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:32:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 00:32:13 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_mshell	mshell;

	if (argc != 1)
		return (printf("Usage: %s\n", argv[0]), 1);
	if (!env || !*env)
		return (printf("Error: Can't load env variables\n"), 1);
	
	/* Initialize mshell structure */
	ft_memset(&mshell, 0, sizeof(t_mshell));
	mshell.config = malloc(sizeof(t_config));
	if (!mshell.config)
		return (printf("Error allocating config\n"), 1);
	load_config(mshell.config, env);
	mshell.envp = save_envp(env);
	if (!mshell.envp)
		return (printf("Error saving envp\n"), 1);
	mshell.exit_code = 0;
	mshell.should_exit = 0;
	
	while (!mshell.should_exit)
	{
		line = readline(mshell.config->prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			add_history(line);
			if (parse_command(&mshell, line) != 0)
				printf("Error parsing command\n");
			else if (mshell.tree)
			{
				printf("===================================================\n");
				print_tree(mshell.tree, 0);
				printf("===================================================\n");
				mshell.exit_code = exec_tree(&mshell);
				if (mshell.exit_code != 0)
					printf("Command exited with code: %d\n", mshell.exit_code);
				ent_free(mshell.tree);
				mshell.tree = NULL;
			}
			if (mshell.config->prompt)
				free(mshell.config->prompt);
			mshell.config->prompt = build_prompt(mshell.config->prompt_raw, mshell.exit_code);
		}
		free(line);
	}
	
	/* Clean up memory */
	if (mshell.raw_command)
		ft_free((void **)&mshell.raw_command);
	if (mshell.expanded_command)
		ft_free((void **)&mshell.expanded_command);
	if (mshell.tree)
		ent_free(mshell.tree);
	if (mshell.config)
	{
		if (mshell.config->prompt)
			free(mshell.config->prompt);
		if (mshell.config->prompt_raw)
			free(mshell.config->prompt_raw);
		free(mshell.config);
	}
	if (mshell.envp)
		envp_clear(mshell.envp);
	return (0);
}
