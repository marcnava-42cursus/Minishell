/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:06 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:25:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include <fcntl.h>
#include <unistd.h>

static const char	**get_config_lines(void)
{
	static const char	*lines[] = {
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

	return (lines);
}

int	create_default_config(void)
{
	int	fd;
	int	result;

	fd = open(CONFIG_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	result = write_config_lines(fd, get_config_lines());
	close(fd);
	return (result);
}
