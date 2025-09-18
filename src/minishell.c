/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:32:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/08 23:52:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_mshell	mshell;

	if (argc != 1)
		return (printf("Usage: %s\n", argv[0]), 1);
	if (!env || !*env)
		return (printf("Error: Can't load env variables\n"), 1);
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
	
	// Configurar manejo de señales
	setup_parent_signals();
	
	if (mshell.config->use_suggestions)
	{
		mshell.suggestions = suggestion_init(mshell.config->prompt);
		if (!mshell.suggestions)
			mshell.config->use_suggestions = 0;
	}
	else
		mshell.suggestions = NULL;
	while (!mshell.should_exit)
	{
		if (mshell.config->use_suggestions && mshell.suggestions)
			line = suggestion_readline(mshell.suggestions);
		else
		{
			if (isatty(STDIN_FILENO))
				line = readline(mshell.config->prompt);
			else
			{
				char buffer[4096];
				int i = 0;
				char ch;
				int r;
				while (i < 4095 && (r = read(STDIN_FILENO, &ch, 1)) > 0)
				{
					if (ch == '\n')
						break;
					buffer[i++] = ch;
				}
				if (r <= 0 && i == 0)
					line = NULL;
				else
				{
					buffer[i] = '\0';
					line = malloc(i + 1);
					if (line)
					{
						int j = 0;
						while (j <= i)
						{
							line[j] = buffer[j];
							j++;
						}
					}
				}
			}
		}
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		// Verificar si se ha recibido una señal
		int signal_exit_code = check_signal_exit_code();
		if (signal_exit_code != 0)
			mshell.exit_code = signal_exit_code;
		
		if (*line)
		{
			add_history(line);
			if (parse_command(&mshell, line) != 0)
			{
				if (isatty(STDIN_FILENO))
					printf("Error parsing command\n");
			}
			else if (mshell.tree)
			{ 
//				printf("===================================================\n");
//				print_tree(mshell.tree, 0);
//				printf("===================================================\n");
				mshell.exit_code = exec_tree(&mshell);
				if (mshell.exit_code != 0 && isatty(STDIN_FILENO))
					printf("Command exited with code: %d\n", mshell.exit_code);
				ent_free(mshell.tree);
				mshell.tree = NULL;
			}
			if (mshell.config->prompt)
				ft_free((void **)&mshell.config->prompt);
			mshell.config->prompt = build_prompt(mshell.config->prompt_raw,
					mshell.exit_code);
			/* Update suggestions prompt if enabled */
			if (mshell.config->use_suggestions && mshell.suggestions)
				suggestion_update_prompt(mshell.suggestions, mshell.config->prompt);
		}
		ft_free((void **)&line);
	}
	if (mshell.raw_command)
		ft_free((void **)&mshell.raw_command);
	if (mshell.expanded_command)
		ft_free((void **)&mshell.expanded_command);
	if (mshell.tree)
		ent_free(mshell.tree);
	if (mshell.config)
	{
		if (mshell.config->prompt)
			ft_free((void **)&mshell.config->prompt);
		if (mshell.config->prompt_raw)
			ft_free((void **)&mshell.config->prompt_raw);
		ft_free((void **)&mshell.config);
	}
	if (mshell.envp)
		envp_clear(mshell.envp);
	if (mshell.suggestions)
		suggestion_cleanup(mshell.suggestions);
	return (mshell.exit_code);
}
