/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 01:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 00:37:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/**
 * @brief Find the full path of a command in PATH environment variable
 * 
 * @param cmd The command to find
 * @param envp Environment variables list
 * @return Full path to command if found, NULL otherwise
 */
char	*find_command_path(char *cmd, t_envp *envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = envp_get_value(envp, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			ft_free_matrix((void **)paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
		{
			ft_free_matrix((void **)paths);
			return (NULL);
		}
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free_matrix((void **)paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_matrix((void **)paths);
	return (NULL);
}

/**
 * @brief Convert t_envp linked list to char** array for execve
 * 
 * @param envp Environment variables linked list
 * @return Array of environment strings, NULL on failure
 */
char	**envp_to_array(t_envp *envp)
{
	t_envp	*current;
	char	**env_array;
	char	*temp;
	int		count;
	int		i;

	if (!envp)
		return (NULL);
	count = 0;
	current = envp;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = envp;
	i = 0;
	while (current && i < count)
	{
		temp = ft_strjoin(current->key, "=");
		if (!temp)
		{
			free_env_array(env_array);
			return (NULL);
		}
		if (current->value)
			env_array[i] = ft_strjoin(temp, current->value);
		else
			env_array[i] = ft_strjoin(temp, "");
		free(temp);
		if (!env_array[i])
		{
			free_env_array(env_array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

/**
 * @brief Free an environment array created by envp_to_array
 * 
 * @param env_array Array to free
 */
void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

/**
 * @brief Remove outer quotes from a string if they are paired
 * 
 * @param str The string to process
 * @return A new string without outer quotes, or copy of original if no quotes
 */
static char	*remove_quotes(const char *str)
{
	size_t	len;
	char	first;
	char	last;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	first = str[0];
	last = str[len - 1];
	if ((first == '"' && last == '"') || (first == '\'' && last == '\''))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

/**
 * @brief Process argv array to remove quotes from arguments
 * 
 * @param argv Array of arguments to process
 * @return New array with quotes removed, or NULL on error
 */
char	**process_argv_quotes(char **argv)
{
	char	**new_argv;
	int		count;
	int		i;

	if (!argv)
		return (NULL);
	count = 0;
	while (argv[count])
		count++;
	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_argv[i] = remove_quotes(argv[i]);
		if (!new_argv[i])
		{
			while (--i >= 0)
				free(new_argv[i]);
			free(new_argv);
			return (NULL);
		}
		i++;
	}
	new_argv[count] = NULL;
	return (new_argv);
}

/**
 * @brief Free argv array created by process_argv_quotes
 * 
 * @param argv Array to free
 */
void	free_processed_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
