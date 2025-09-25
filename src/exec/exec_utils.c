/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:41:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 20:38:59 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	match_builtin_name(const char *name);
static void	free_string_array(char **arr);

static char	*dup_last_arg(char **argv)
{
	char	**unq;
	int		c;
	char	*res;

	if (!argv || !argv[0])
		return (NULL);
	c = 0;
	while (argv[c])
		c++;
	if (c == 0)
		return (NULL);
	unq = process_argv_quotes(argv);
	if (!unq)
		return (NULL);
	res = ft_strdup(unq[c - 1]);
	free_string_array(unq);
	return (res);
}

static char	*dup_last_export_assign(char **argv)
{
	char	**unq;
	int		i;
	char	*dup;

	if (!argv || !argv[0])
		return (NULL);
	unq = process_argv_quotes(argv);
	if (!unq)
		return (NULL);
	if (!unq[0] || ft_strcmp(unq[0], "export") != 0)
		return (free_string_array(unq), NULL);
	dup = NULL;
	i = 1;
	while (unq[i])
	{
		if (ft_strchr(unq[i], '='))
		{
			ft_free((void **)&dup);
			dup = ft_strdup(unq[i]);
		}
		i++;
	}
	free_string_array(unq);
	return (dup);
}

static char	*dup_from_pipeline_commands(t_ent **cmds, int n)
{
	int		i;
	char	*assign;
	char	*fallback;

	assign = NULL;
	i = 0;
	while (i < n)
	{
		assign = dup_last_export_assign(cmds[i]->argv);
		if (assign)
			return (assign);
		i++;
	}
	fallback = dup_last_arg(cmds[n - 1]->argv);
	return (fallback);
}

static char	*compute_underscore_value(t_ent *root)
{
	int		count;
	t_ent	**cmds;
	char	*val;

	if (!root)
		return (NULL);
	count = count_pipeline_commands(root);
	if (count <= 0)
		return (NULL);
	if (count == 1)
	{
		val = dup_last_export_assign(root->argv);
		if (val)
			return (val);
		return (dup_last_arg(root->argv));
	}
	cmds = malloc(sizeof(t_ent *) * count);
	if (!cmds)
		return (NULL);
	collect_pipeline_commands(root, cmds, count);
	val = dup_from_pipeline_commands(cmds, count);
	ft_free((void **)&cmds);
	return (val);
}

void	ms_update_underscore_after_exec(t_mshell *mshell)
{
	char	*val;

	if (!mshell || !mshell->tree)
		return ;
	val = compute_underscore_value(mshell->tree);
	if (!val)
		return ;
	envp_set_value(&(mshell->envp), "_", val);
	ft_free((void **)&val);
}

static int	match_builtin_name(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp((char *)name, "cd") == 0)
		return (1);
	if (ft_strcmp((char *)name, "echo") == 0)
		return (1);
	if (ft_strcmp((char *)name, "env") == 0)
		return (1);
	if (ft_strcmp((char *)name, "exit") == 0)
		return (1);
	if (ft_strcmp((char *)name, "export") == 0)
		return (1);
	if (ft_strcmp((char *)name, "pwd") == 0)
		return (1);
	if (ft_strcmp((char *)name, "unset") == 0)
		return (1);
	return (0);
}

static void	free_string_array(char **arr)
{
	int		index;

	if (!arr)
		return ;
	index = 0;
	while (arr[index])
	{
		free(arr[index]);
		index++;
	}
	free(arr);
}

/**
 * @brief Check if a command is a builtin
 * 
 * @param cmd The command to check
 * @return 1 if it's a builtin, 0 otherwise
 */
int	is_builtin(char *cmd)
{
	char	*argv[2];
	char	**unquoted;
	int		result;

	if (!cmd)
		return (0);
	argv[0] = cmd;
	argv[1] = NULL;
	unquoted = process_argv_quotes(argv);
	if (!unquoted)
		return (0);
	result = match_builtin_name(unquoted[0]);
	free_string_array(unquoted);
	return (result);
}

/**
 * @brief Apply redirections for a node
 * 
 * @param node The node containing redirection information
 * @return 0 on success, 1 on error
 */
int	apply_redirections(t_ent *node)
{
	if (node->fd_in == -2 || node->fd_out == -2)
		return (1);
	if (node->fd_in != -1)
	{
		if (dup2(node->fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"), 1);
		close(node->fd_in);
	}
	if (node->fd_out != -1)
	{
		if (dup2(node->fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);
		close(node->fd_out);
	}
	return (0);
}

void	ms_update_underscore(t_mshell *mshell, char **argv)
{
	char	**unquoted;
	int		count;

	if (!mshell || !argv)
		return ;
	count = 0;
	while (argv[count])
		count++;
	if (count == 0)
		return ;
	unquoted = process_argv_quotes(argv);
	if (!unquoted)
		return ;
	if (unquoted[count - 1])
		envp_set_value(&(mshell->envp), "_", unquoted[count - 1]);
	ft_free_matrix((void **)unquoted);
}
