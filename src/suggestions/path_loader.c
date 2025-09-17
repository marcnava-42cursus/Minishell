/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_loader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:37:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 11:37:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"

static void	add_executables_from_dir(t_cmd_list *list, const char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			full_path[4096];
	size_t			dir_len;
	size_t			name_len;

	dir = opendir(dir_path);
	if (!dir)
		return ;
	dir_len = sug_strlen(dir_path);
	entry = readdir(dir);
	while (entry != NULL)
	{
		name_len = sug_strlen(entry->d_name);
		if (dir_len + 1 + name_len + 1 >= sizeof(full_path))
			continue ;
		sug_strcpy(full_path, dir_path);
		sug_strcat(full_path, "/");
		sug_strcat(full_path, entry->d_name);
		if (access(full_path, X_OK) == 0)
			cmdlist_add(list, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
}

static void	process_path_directory(t_cmd_list *list, const char *dir_buffer)
{
	if (!dir_buffer || !*dir_buffer)
		return ;
	add_executables_from_dir(list, dir_buffer);
}

static void	process_path_char(t_cmd_list *list, char *dir_buffer,
				size_t *dir_index, char c)
{
	if (c != ':' && c != '\0')
	{
		if (*dir_index + 1 < 4096)
			dir_buffer[(*dir_index)++] = c;
	}
	else
	{
		dir_buffer[*dir_index] = '\0';
		if (*dir_index > 0)
			process_path_directory(list, dir_buffer);
		*dir_index = 0;
	}
}

void	cmdlist_load_from_path(t_cmd_list *list, char *path_env)
{
	char	dir_buffer[4096];
	size_t	dir_index;
	size_t	i;
	char	c;

	if (!list || !path_env)
		return ;
	dir_index = 0;
	i = 0;
	while (1)
	{
		c = path_env[i];
		process_path_char(list, dir_buffer, &dir_index, c);
		if (c == '\0')
			break ;
		i++;
	}
}
