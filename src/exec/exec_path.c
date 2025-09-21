/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:58:02 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 23:58:02 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	*try_direct_cmd(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	**get_path_split(t_envp *envp)
{
	char	*path_env;

	path_env = envp_get_value(envp, "PATH");
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

static char	*search_paths_for_cmd(char *cmd, char **paths)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		ft_free((void **)&temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		ft_free((void **)&full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_envp *envp)
{
	char	**paths;
	char	*direct;
	char	*found;

	direct = try_direct_cmd(cmd);
	if (direct)
		return (direct);
	paths = get_path_split(envp);
	if (!paths)
		return (NULL);
	found = search_paths_for_cmd(cmd, paths);
	ft_free_matrix((void **)paths);
	return (found);
}
