/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keywords_system.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:26:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*get_hostname(void)
{
	static char	hostname[256];

	if (gethostname(hostname, sizeof(hostname)) == 0)
		return (hostname);
	return ("unknown");
}

char	*get_whoami(void)
{
	struct passwd	*pw;

	pw = getpwuid(getuid());
	if (pw)
		return (pw->pw_name);
	return ("unknown");
}

char	*get_pwd(void)
{
	static char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		return (cwd);
	return ("unknown");
}

char	*get_jobs(void)
{
	return ("0");
}

char	*get_battery(void)
{
	return ("100%");
}
