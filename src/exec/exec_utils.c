/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:41:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/07 18:41:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/**
 * @brief Check if a command is a builtin
 * 
 * @param cmd The command to check
 * @return 1 if it's a builtin, 0 otherwise
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

/**
 * @brief Execute a builtin command
 * 
 * @param node The command node to execute
 * @param envp Pointer to environment variables
 * @param config Configuration structure
 * @return Exit code of the builtin
 */
int	execute_builtin(t_ent *node, t_envp **envp, t_config *config)
{
	char	*cmd;
	char	**processed_argv;
	int		result;

	if (!node || !node->argv || !node->argv[0])
		return (1);
	
	/* Process argv to remove quotes for builtins too */
	processed_argv = process_argv_quotes(node->argv);
	if (!processed_argv)
		return (1);
	
	cmd = processed_argv[0];
	result = 1; /* Default error result */
	
	if (ft_strcmp(cmd, "cd") == 0)
	{
		if (processed_argv[1])
			result = msh_exec_bt_cd(envp, processed_argv[1]);
		else
			result = msh_exec_bt_cd(envp, envp_get_value(*envp, "HOME"));
	}
	else if (ft_strcmp(cmd, "echo") == 0)
	{
		/* For echo, we need to create a temporary node with processed argv */
		t_ent temp_node = *node;
		temp_node.argv = processed_argv;
		result = msh_exec_bt_echo(&temp_node);
	}
	else if (ft_strcmp(cmd, "env") == 0)
		result = msh_exec_bt_env(*envp);
	else if (ft_strcmp(cmd, "exit") == 0)
		result = msh_exec_bt_exit();
	else if (ft_strcmp(cmd, "export") == 0)
	{
		if (processed_argv[1])
		{
			char *eq = ft_strchr(processed_argv[1], '=');
			if (eq)
			{
				*eq = '\0';
				result = msh_exec_bt_export(envp, processed_argv[1], eq + 1);
			}
			else
				result = msh_exec_bt_export(envp, processed_argv[1], "");
		}
		else
			result = msh_exec_bt_env(*envp); // export without args shows env
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
		result = msh_exec_bt_pwd(*envp);
	else if (ft_strcmp(cmd, "unset") == 0)
	{
		if (processed_argv[1])
			result = msh_exec_bt_unset(envp, processed_argv[1]);
		else
			result = 0;
	}
	
	/* Clean up processed argv */
	free_processed_argv(processed_argv);
	(void)config; // Suppress unused parameter warning
	return (result);
}

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
	// If cmd contains '/', it's already a path
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
	
	// Count environment variables
	count = 0;
	current = envp;
	while (current)
	{
		count++;
		current = current->next;
	}
	
	// Allocate array
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	
	// Fill array with KEY=VALUE strings
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
		env_array[i] = ft_strjoin(temp, current->value ? current->value : "");
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
		return;
	
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
static char *remove_quotes(const char *str)
{
	size_t len;
	char first, last;

	if (!str)
		return (NULL);
	
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	
	first = str[0];
	last = str[len - 1];
	
	/* Check if we have matching quotes */
	if ((first == '"' && last == '"') || (first == '\'' && last == '\''))
	{
		/* Return substring without quotes */
		return (ft_substr(str, 1, len - 2));
	}
	
	/* No matching quotes, return copy of original */
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
	
	/* Count arguments */
	count = 0;
	while (argv[count])
		count++;
	
	/* Allocate new array */
	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return (NULL);
	
	/* Process each argument */
	for (i = 0; i < count; i++)
	{
		new_argv[i] = remove_quotes(argv[i]);
		if (!new_argv[i])
		{
			/* Cleanup on error */
			while (--i >= 0)
				free(new_argv[i]);
			free(new_argv);
			return (NULL);
		}
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
		return;
	
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
