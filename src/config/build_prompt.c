/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:38 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:11:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*replace_keyword(char *key, int exit_code)
{
	(void)exit_code;
	if (!ft_strcmp(key, "HOSTNAME"))
		return (get_hostname());
	if (!ft_strcmp(key, "WHOAMI"))
		return (get_whoami());
	if (!ft_strcmp(key, "PWD"))
		return (get_pwd());
	if (!ft_strcmp(key, "GITBRANCH"))
		return (get_gitbranch());
	if (!ft_strcmp(key, "JOBS"))
		return (get_jobs());
	if (!ft_strcmp(key, "BATTERY"))
		return (get_battery());
	return ("");
}

char	*build_prompt(char *raw, int exit_code)
{
	char	*result;
	char	keyword[PROMPT_BUF];
	char	color[PROMPT_BUF];
	int		i;

	result = ft_strdup("");
	if (!result || !raw)
		return (result);
	i = 0;
	while (raw[i])
	{
		if (raw[i] == '{')
		{
			i++;
			if (!parse_keyword_block(raw, &i, keyword, color)
				|| !build_keyword_output(&result, keyword, exit_code, color))
				return (result);
		}
		else if (!append_single_char(&result, raw, &i))
			return (result);
	}
	return (result);
}
