/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:50:41 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:26:02 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

static int	open_config_file(void)
{
	int	fd;

	fd = open(CONFIG_FILE, O_RDONLY);
	if (fd < 0)
	{
		if (create_default_config())
			return (-1);
		fd = open(CONFIG_FILE, O_RDONLY);
	}
	return (fd);
}

static void	parse_config_file(int fd, char **prompt_line,
	char **suggestions_line)
{
	char	*line;

	*prompt_line = NULL;
	*suggestions_line = NULL;
	line = read_single_line(fd);
	while (line != NULL)
	{
		process_config_line(line, prompt_line, suggestions_line);
		ft_free((void **)&line);
		line = read_single_line(fd);
	}
}

int	load_config(t_config *config, char **envp)
{
	int		fd;
	char	*prompt_line;
	char	*suggestions_line;

	(void)envp;
	fd = open_config_file();
	if (fd < 0)
		return (1);
	parse_config_file(fd, &prompt_line, &suggestions_line);
	close(fd);
	if (!prompt_line)
		return (1);
	config->prompt_raw = prompt_line;
	config->prompt = build_prompt(config->prompt_raw, 0, NULL);
	if (suggestions_line && ft_strncmp(suggestions_line, "enabled", 7) == 0)
		config->use_suggestions = 1;
	else
		config->use_suggestions = 0;
	if (suggestions_line)
		ft_free((void **)&suggestions_line);
	return (0);
}

void	free_config(t_config *config)
{
	if (config->prompt_raw)
		ft_free((void **)&config->prompt_raw);
	if (config->prompt)
		ft_free((void **)&config->prompt);
}
