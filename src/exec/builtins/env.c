/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:23:35 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/01 20:33:27 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

int	msh_exec_bt_env(t_envp *envp)
{
	t_envp	*curr;

	curr = envp;
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	return (0);
}
