/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:38 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/16 21:31:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

static char *append(char *dst, const char *src)
{
	char *tmp;
	size_t len_dst = ft_strlen(dst);
	size_t len_src = ft_strlen(src);

	tmp = malloc(len_dst + len_src + 1);
	if (!tmp)
		return (NULL);
	ft_strlcpy(tmp, dst, len_dst + 1);
	ft_strlcat(tmp, src, len_dst + len_src + 1);
	free(dst);
	return (tmp);
}

char *replace_keyword(char *key, int exit_code)
{
	if (exit_code)
		exit_code = 0;
	if (!ft_strcmp(key, "HOSTNAME"))
		return (get_hostname());
	if (!ft_strcmp(key, "WHOAMI"))
		return (get_whoami());
	if (!ft_strcmp(key, "PWD"))
		return (get_pwd());
	if (!ft_strcmp(key, "GITBRANCH"))
		return (get_gitbranch());
	// if (!ft_strcmp(key, "TIME"))
	// 	return (get_time());
	// if (!ft_strcmp(key, "DATETIME"))
	// 	return (get_datetime());
	// if (!ft_strcmp(key, "EXITCODE"))
	// 	return (get_exitcode(exit_code));
	if (!ft_strcmp(key, "JOBS"))
		return (get_jobs());
	if (!ft_strcmp(key, "BATTERY"))
		return (get_battery());
	return ("");
}

char *build_prompt(char *raw, int exit_code)
{
	char *result = ft_strdup("");
	int i = 0;

	while (raw[i])
	{
		if (raw[i] == '{')
		{
			i++;
			char keyword[64];
			char color[64];
			int k = 0;
			while (raw[i] && raw[i] != ' ' && raw[i] != '}')
				keyword[k++] = raw[i++];
			keyword[k] = '\0';

			if (raw[i] == ' ')
			{
				i++;
				int c = 0;
				while (raw[i] && raw[i] != '}')
					color[c++] = raw[i++];
				color[c] = '\0';
			}
			else
				color[0] = '\0';
			if (raw[i] == '}')
				i++;

			result = append(result, get_color_code(color));
			result = append(result, replace_keyword(keyword, exit_code));
			result = append(result, reset_color());
		}
		else
		{
			char tmp[2] = {raw[i], '\0'};
			result = append(result, tmp);
			i++;
		}
	}
	return (result);
}
