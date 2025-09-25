/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:38 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 06:28:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*replace_keyword(char *key, int exit_code, t_envp *env)
{
	(void)exit_code;
	if (!ft_strcmp(key, "HOSTNAME"))
		return (get_hostname());
	if (!ft_strcmp(key, "WHOAMI"))
		return (get_whoami());
	if (!ft_strcmp(key, "PWD"))
		return (get_pwd(env));
	if (!ft_strcmp(key, "GITBRANCH"))
		return (get_gitbranch());
	return ("");
}

char	*build_prompt(t_mshell *ms)
{
	char	*result;
	char	keyword[PROMPT_BUF];
	char	color[PROMPT_BUF];
	int		i;

	result = ft_strdup("");
	if (!result || !ms->config->prompt_raw)
		return (result);
	i = 0;
	while (ms->config->prompt_raw[i])
	{
		if (ms->config->prompt_raw[i] == '{')
		{
			i++;
			if (!parse_keyword_block(ms->config->prompt_raw, &i, keyword, color)
				|| !build_keyword_output(&result, keyword, ms,
					color))
				return (result);
		}
		else if (!append_single_char(&result, ms->config->prompt_raw, &i))
			return (result);
	}
	return (result);
}
