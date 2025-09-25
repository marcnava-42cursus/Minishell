/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keywords_system.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 06:12:32 by marcnava         ###   ########.fr       */
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

static char	*lookup_pwd_env(t_envp *env)
{
	while (env)
	{
		if (env->key && ft_strcmp(env->key, "PWD") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*get_pwd(t_envp *env)
{
	static char	cwd[1024];
	char		*logical;

	if (getcwd(cwd, sizeof(cwd)))
		return (cwd);
	logical = lookup_pwd_env(env);
	if (logical)
		return (logical);
	return ("unknown");
}
