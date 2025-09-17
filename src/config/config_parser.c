/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:22:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:26:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*read_single_line(int fd)
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

char	*extract_config_value(char *line)
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

int	process_config_line(char *line, char **prompt_line, char **suggestions_line)
{
	if (!line)
		return (0);
	if (ft_strncmp(line, "MINISHELLPROMPT", 15) == 0)
		*prompt_line = extract_config_value(line);
	else if (ft_strncmp(line, "SUGGESTIONS", 11) == 0)
		*suggestions_line = extract_config_value(line);
	return (1);
}
