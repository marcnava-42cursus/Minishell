/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:49:05 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 06:14:32 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"
#include "utils.h"
#include <errno.h>
#include <string.h>

static char	*build_logical_pwd(const char *old_pwd, const char *target_path)
{
	char	*tmp;
	char	*out;

	if (!target_path)
		return (NULL);
	if (target_path[0] == '/' || !old_pwd)
		return (ft_strdup(target_path));
	if (target_path[0] == '\0')
		return (ft_strdup(old_pwd));
	if (ft_strcmp((char *)target_path, ".") == 0)
		return (ft_strdup(old_pwd));
	if (old_pwd[0] == '\0')
		return (ft_strdup(target_path));
	if (old_pwd[ft_strlen(old_pwd) - 1] == '/')
		return (ft_strjoin(old_pwd, target_path));
	tmp = ft_strjoin(old_pwd, "/");
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, target_path);
	ft_free((void **)&tmp);
	return (out);
}

static void	print_getcwd_error(int err)
{
	print_err2("minishell: cd: error retrieving current directory: ",
		"getcwd: cannot access parent directories: ", strerror(err));
	print_err2("\n", NULL, NULL);
}

static char	*get_oldpwd(t_envp *env)
{
	char	*pwd;

	pwd = envp_get_value(env, "PWD");
	if (!pwd)
		return (getcwd(NULL, 0));
	return (ft_strdup(pwd));
}

static int	resolve_target_path(t_envp *env, const char *path,
	char **out_target)
{
	char	*val;

	if (!path || *path == '\0')
	{
		val = envp_get_value(env, "HOME");
		if (!val)
			return (ft_putstr_fd("msh: cd: HOME not set\n", 2), 1);
		*out_target = val;
		return (0);
	}
	if (ft_strcmp((char *)path, "-") == 0)
	{
		val = envp_get_value(env, "OLDPWD");
		if (!val)
			return (ft_putstr_fd("msh: cd: OLDPWD not set\n", 2), 1);
		ft_putstr_fd(val, 1);
		ft_putstr_fd("\n", 1);
		*out_target = val;
		return (0);
	}
	*out_target = (char *)path;
	return (0);
}

static int	chdir_and_update_env(t_envp **envp, const char *target_path,
	char *old_pwd)
{
	char	*new_pwd;
	char	*logical_pwd;
	int		err;

	if (chdir(target_path) != 0)
	{
		err = errno;
		print_err2("msh: cd: ", target_path, ": ");
		print_err2(strerror(err), "\n", NULL);
		ft_free((void **)&old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		err = errno;
		logical_pwd = build_logical_pwd(old_pwd, target_path);
		if (!logical_pwd)
		{
			print_err2("minishell: cd: allocation error\n", NULL, NULL);
			ft_free((void **)&old_pwd);
			return (1);
		}
		print_getcwd_error(err);
		envp_set_value(envp, "OLDPWD", old_pwd);
		envp_set_value(envp, "PWD", logical_pwd);
		ft_free((void **)&logical_pwd);
		ft_free((void **)&old_pwd);
		return (0);
	}
	envp_set_value(envp, "OLDPWD", old_pwd);
	envp_set_value(envp, "PWD", new_pwd);
	ft_free((void **)&old_pwd);
	ft_free((void **)&new_pwd);
	return (0);
}

/**
 * @brief Change directory and update PWD and OLDPWD environment variables.
 * 
 * @param envp Pointer to the environment variables list.
 * @param path The path to change to. If NULL, changes to HOME. If "-",
 *             changes to OLDPWD.
 * @return 0 on success, 1 on failure.
 */
int	msh_exec_bt_cd(t_envp **envp, const char *path)
{
	char	*old_pwd;
	char	*target_path;

	if (!envp)
		return (1);
	old_pwd = get_oldpwd(*envp);
	if (!old_pwd)
		return (1);
	if (resolve_target_path(*envp, path, &target_path))
	{
		ft_free((void **)&old_pwd);
		return (1);
	}
	return (chdir_and_update_env(envp, target_path, old_pwd));
}
