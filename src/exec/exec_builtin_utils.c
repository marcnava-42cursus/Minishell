/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 02:45:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	handle_cd_builtin(char **processed_argv, t_envp **envp)
{
	if (processed_argv[1])
		return (msh_exec_bt_cd(envp, processed_argv[1]));
	else
		return (msh_exec_bt_cd(envp, envp_get_value(*envp, "HOME")));
}

int	handle_echo_builtin(t_ent *node, char **processed_argv)
{
	t_ent	temp_node;

	temp_node = *node;
	temp_node.argv = processed_argv;
	return (msh_exec_bt_echo(&temp_node));
}

int	handle_export_builtin(char **processed_argv, t_envp **envp)
{
	char	*eq;

	if (!processed_argv[1])
		return (msh_exec_bt_env(*envp));
	eq = ft_strchr(processed_argv[1], '=');
	if (eq)
	{
		*eq = '\0';
		return (msh_exec_bt_export(envp, processed_argv[1], eq + 1));
	}
	else
		return (msh_exec_bt_export(envp, processed_argv[1], ""));
}

int	handle_unset_builtin(char **processed_argv, t_envp **envp)
{
	if (processed_argv[1])
		return (msh_exec_bt_unset(envp, processed_argv[1]));
	else
		return (0);
}

int	handle_exit_builtin(char **processed_argv, int last_exit_code)
{
	return (msh_exec_bt_exit(processed_argv, last_exit_code));
}
