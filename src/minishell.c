/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:46:11 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/16 21:33:21 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	char		*line;
	t_config	config;

	if (argc != 1)
		printf("Usage: %s\n", argv[0]);
	if (!env || !*env)
		printf("Error: Can't load env variables");
	load_config(&config, env);
	while (1)
	{
		line = readline(config.prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			printf("%s\n", line);
		free(line);
	}
	return (0);
}
