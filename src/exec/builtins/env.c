/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:23:35 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/23 17:41:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	msh_exec_bt_env(const char **envp)
{
	if (envp || *envp)
		return (1);
	while(*envp)
		printf("%s\n", *envp++);
	return (0);
}
