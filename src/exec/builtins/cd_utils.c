/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 00:42:15 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/26 01:06:27 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"
#include "utils.h"
#include <errno.h>
#include <string.h>

static int	chech_new_pwd(char *pwd)
{
	if (pwd && pwd[0] == '/')
		return (0);
	return (1);
}

static int	update_env_success(t_envp **envp, char *old_pwd, char *new_pwd)
{
	envp_set_value(envp, "OLDPWD", old_pwd);
	envp_set_value(envp, "PWD", new_pwd);
	ft_free((void **)&old_pwd);
	ft_free((void **)&new_pwd);
	return (0);
}

static int	update_env_logical(t_envp **envp, const char *target_path,
				char *old_pwd, int err)
{
	char	*logical_pwd;

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

int	chdir_and_update_env(t_envp **envp, const char *target_path, char *old_pwd)
{
	char	*new_pwd;
	int		err;

	if (chdir(target_path) != 0)
	{
		err = errno;
		print_err2("minishell: cd: ", target_path, ": ");
		print_err2(strerror(err), "\n", NULL);
		ft_free((void **)&old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (chech_new_pwd(new_pwd))
		return (update_env_logical(envp, target_path, old_pwd, errno));
	return (update_env_success(envp, old_pwd, new_pwd));
}
