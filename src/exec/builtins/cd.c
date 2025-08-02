/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:49:05 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/02 13:43:49 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

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
	char	*new_pwd;
	char	*target_path;

	if (!envp)
		return (1);
	// Get current PWD before changing
	old_pwd = envp_get_value(*envp, "PWD");
	if (!old_pwd)
	{
		old_pwd = getcwd(NULL, 0);
		if (!old_pwd)
			return (1);
	}
	else
		old_pwd = ft_strdup(old_pwd);
	// Determine target path
	if (!path || *path == '\0')
	{
		target_path = envp_get_value(*envp, "HOME");
		if (!target_path)
		{
			ft_putstr_fd("msh: cd: HOME not set\n", 2);
			ft_free((void **)&old_pwd);
			return (1);
		}
	}
	else if (ft_strcmp((char *)path, "-") == 0)
	{
		target_path = envp_get_value(*envp, "OLDPWD");
		if (!target_path)
		{
			ft_putstr_fd("msh: cd: OLDPWD not set\n", 2);
			ft_free((void **)&old_pwd);
			return (1);
		}
		// Print the directory we're changing to (bash behavior)
		ft_putstr_fd(target_path, 1);
		ft_putstr_fd("\n", 1);
	}
	else
		target_path = (char *)path;
	// Try to change directory
	if (chdir(target_path) != 0)
	{
		perror("msh: cd");
		ft_free((void **)&old_pwd);
		return (1);
	}
	// Get new PWD
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_free((void **)&old_pwd);
		return (1);
	}
	// Update OLDPWD
	envp_set_value(envp, "OLDPWD", old_pwd);
	// Update PWD
	envp_set_value(envp, "PWD", new_pwd);
	// Clean up
	ft_free((void **)&old_pwd);
	free(new_pwd);
	return (0);
}
