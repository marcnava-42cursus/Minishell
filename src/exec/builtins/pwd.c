/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:37:47 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 01:22:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

/**
 * @brief Print the current working directory from envp.
 * 
 * @param envp The environment variables list.
 * @return 0 on success, 1 on failure.
 */
int	msh_exec_bt_pwd(t_envp *envp)
{
	char	*pwd;
	char	*cwd;

	pwd = envp_get_value(envp, "PWD");
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("msh: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
