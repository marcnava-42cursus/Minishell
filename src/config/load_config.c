/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:50:41 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 14:13:37 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include <fcntl.h>
#include <unistd.h>

static char	*extract_value(char *line)
{
	char	*colon;
	char	*value;
	size_t	len;

	colon = ft_strchr(line, ':');
	if (!colon)
		return (NULL);
	colon++;
	while (*colon == ' ' || *colon == '\t')
		colon++;
	value = ft_strdup(colon);
	if (!value)
		return (NULL);
	len = ft_strlen(value);
	if (len > 0 && value[len - 1] == '\n')
		value[len - 1] = '\0';
	return (value);
}

static char	*read_line(int fd)
{
	char	buffer[MAX_LINE_LEN];
	int		i;
	ssize_t	bytes;
	char	c;

	i = 0;
	while (i < MAX_LINE_LEN - 1)
	{
		bytes = read(fd, &c, 1);
		if (bytes <= 0)
			break ;
		if (c == '\n')
			break ;
		buffer[i++] = c;
	}
	if (bytes <= 0 && i == 0)
		return (NULL);
	buffer[i] = '\0';
	return (ft_strdup(buffer));
}

int	load_config(t_config *config, char **envp)
{
	int		fd;
	char	*line;
	char	*prompt_line;

	(void)envp;
	fd = open(CONFIG_FILE, O_RDONLY);
	if (fd < 0)
	{
		if (create_default_config())
			return (1);
		fd = open(CONFIG_FILE, O_RDONLY);
	}
	prompt_line = NULL;
	line = read_line(fd);
	while (line != NULL)
	{
		if (ft_strncmp(line, "MINISHELLPROMPT", 15) == 0)
			prompt_line = extract_value(line);
		ft_free((void **)&line);
		line = read_line(fd);
	}
	close(fd);
	if (!prompt_line)
		return (1);
	config->prompt_raw = prompt_line;
	config->prompt = build_prompt(config->prompt_raw, config->exit_code);
	return (0);
}

void	free_config(t_config *config)
{
	if (config->prompt_raw)
		ft_free((void **)&config->prompt_raw);
	if (config->prompt)
		ft_free((void **)&config->prompt);
}
