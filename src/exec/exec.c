/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:42:19 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/03 16:58:58 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	**envp_to_array(t_envp *envp)
{
	char	**env_array;
	int		count;
	t_envp	*current;
	int		i;
	char	*env_var;

	count = 0;
	current = envp;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = ft_calloc(count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	current = envp;
	while (current)
	{
		env_var = ft_calloc(
				ft_strlen(current->key) + ft_strlen(current->value) + 2,
				sizeof(char));
		if (!env_var)
		{
			while (i > 0)
				ft_free((void **)&env_array[--i]);
			ft_free((void **)&env_array);
			return (NULL);
		}
		ft_strlcpy(env_var, current->key,
			ft_strlen(current->key) + ft_strlen(current->value) + 2);
		ft_strlcat(env_var, "=",
			ft_strlen(current->key) + ft_strlen(current->value) + 2);
		ft_strlcat(env_var, current->value,
			ft_strlen(current->key) + ft_strlen(current->value) + 2);
		env_array[i] = env_var;
		i++;
		current = current->next;
	}
	return (env_array);
}

static void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		ft_free((void **)&env_array[i]);
		i++;
	}
	ft_free((void **)&env_array);
}

static char	*get_path_env(t_envp *envp)
{
	t_envp	*current;

	current = envp;
	while (current)
	{
		if (ft_strcmp(current->key, "PATH") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*get_next_path_dir(char *path, int *start)
{
	int		i;
	int		len;
	char	*dir;

	if (!path || *start < 0)
		return (NULL);
	i = *start;
	while (path[i] && path[i] == ':')
		i++;
	if (!path[i])
		return (NULL);
	*start = i;
	while (path[i] && path[i] != ':')
		i++;
	len = i - *start;
	dir = ft_calloc(len + 1, sizeof(char));
	if (!dir)
		return (NULL);
	ft_strlcpy(dir, path + *start, len + 1);
	*start = i;
	return (dir);
}

static char	*find_command_path(char *command, t_envp *envp)
{
	char	*path_env;
	char	*dir;
	char	*full_path;
	int		path_len;
	int		cmd_len;
	int		start;

	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	cmd_len = ft_strlen(command);
	start = 0;
	dir = get_next_path_dir(path_env, &start);
	while (dir)
	{
		path_len = ft_strlen(dir);
		full_path = malloc(path_len + cmd_len + 2);
		if (!full_path)
		{
			ft_free((void **)&dir);
			return (NULL);
		}
		ft_strlcpy(full_path, dir, path_len + cmd_len + 2);
		ft_strlcat(full_path, "/", path_len + cmd_len + 2);
		ft_strlcat(full_path, command, path_len + cmd_len + 2);
		if (access(full_path, X_OK) == 0)
		{
			ft_free((void **)&dir);
			return (full_path);
		}
		ft_free((void **)&full_path);
		ft_free((void **)&dir);
		dir = get_next_path_dir(path_env, &start);
	}
	return (NULL);
}

static int	is_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0
		|| ft_strcmp(command, "echo") == 0
		|| ft_strcmp(command, "env") == 0
		|| ft_strcmp(command, "exit") == 0
		|| ft_strcmp(command, "export") == 0
		|| ft_strcmp(command, "pwd") == 0
		|| ft_strcmp(command, "unset") == 0)
		return (1);
	return (0);
}

static int	execute_builtin(t_ent *cmd_node, t_envp **envp)
{
	char	**argv;
	char	*equals;

	argv = cmd_node->argv;
	if (ft_strcmp(argv[0], "cd") == 0)
		return (msh_exec_bt_cd(argv[1]));
	else if (ft_strcmp(argv[0], "echo") == 0)
		return (msh_exec_bt_echo(cmd_node));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (msh_exec_bt_env(*envp));
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (msh_exec_bt_exit());
	else if (ft_strcmp(argv[0], "export") == 0)
	{
		if (argv[1])
		{
			equals = ft_strchr(argv[1], '=');
			if (equals)
			{
				*equals = '\0';
				return (msh_exec_bt_export(envp, argv[1], equals + 1));
			}
			else
				return (msh_exec_bt_export(envp, argv[1], NULL));
		}
		return (msh_exec_bt_export(envp, NULL, NULL));
	}
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (msh_exec_bt_pwd());
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (msh_exec_bt_unset(envp, argv[1]));
	return (1);
}

static int	execute_command(t_ent *cmd_node, t_envp **envp)
{
	pid_t	pid;
	int		status;
	char	**env_array;
	char	*cmd_path;

	if (!cmd_node->argv || !cmd_node->argv[0])
		return (1);
	if (is_builtin(cmd_node->argv[0]))
		return (execute_builtin(cmd_node, envp));
	env_array = envp_to_array(*envp);
	if (!env_array)
		return (1);
	cmd_path = find_command_path(cmd_node->argv[0], *envp);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", cmd_node->argv[0]);
		free_env_array(env_array);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_free((void **)&cmd_path);
		free_env_array(env_array);
		return (1);
	}
	else if (pid == 0)
	{
		if (execve(cmd_path, cmd_node->argv, env_array) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else
	{
		wait(&status);
		ft_free((void **)&cmd_path);
		free_env_array(env_array);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
	return (0);
}

int	exec(t_ent *tree, t_envp *envp)
{
	if (!tree)
		return (0);
	if (tree->type == NODE_COMMAND)
		return (execute_command(tree, &envp));
	printf("minishell: unsupported operation\n");
	return (1);
}
