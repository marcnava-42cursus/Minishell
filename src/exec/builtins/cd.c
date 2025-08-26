/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:49:05 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 01:26:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

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

	if (chdir(target_path) != 0)
	{
		perror("msh: cd");
		ft_free((void **)&old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_free((void **)&old_pwd);
		return (1);
	}
	envp_set_value(envp, "OLDPWD", old_pwd);
	envp_set_value(envp, "PWD", new_pwd);
	ft_free((void **)&old_pwd);
	free(new_pwd);
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
