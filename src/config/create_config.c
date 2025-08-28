/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:06 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 18:26:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include <fcntl.h>
#include <unistd.h>

int	create_default_config(void)
{
	int			fd;
	int			i;
	const char	*lines[] = {
		"# Minishell configuration file\n",
		"# Available keywords:\n",
		"# HOSTNAME    - Hostname of the machine\n",
		"# WHOAMI      - Current username\n",
		"# PWD         - Current working directory\n",
		"#? GITBRANCH  - Current git branch\n\n",
		"# Suggestions configuration (enabled/disabled)\n\n",
		"SUGGESTIONS: disabled\n",
		"MINISHELLPROMPT: ~ {PWD GREEN} $ \n",
		NULL
	};

	fd = open(CONFIG_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	i = 0;
	while (lines[i])
	{
		write(fd, lines[i], ft_strlen(lines[i]));
		i++;
	}
	close(fd);
	return (0);
}
