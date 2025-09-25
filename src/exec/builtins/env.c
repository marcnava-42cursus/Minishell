/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:23:35 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 06:13:48 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"
#include "utils.h"

int	msh_exec_bt_env(char **argv, t_envp *envp)
{
	t_envp	*curr;

	if (argv && argv[1])
	{
		print_err2("minishell: env: ", argv[1],
			": No such file or directory\n");
		return (127);
	}
	curr = envp;
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	return (0);
}
