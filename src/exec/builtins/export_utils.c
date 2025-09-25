/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarcell <jmarcell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 00:36:17 by jmarcell          #+#    #+#             */
/*   Updated: 2025/09/26 00:37:09 by jmarcell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

int	count_visible(t_envp *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		if (ft_strcmp(envp->key, "_") != 0)
			count++;
		envp = envp->next;
	}
	return (count);
}

void	fill_ptrs(t_envp *envp, t_envp **arr)
{
	int	i;

	i = 0;
	while (envp)
	{
		if (ft_strcmp(envp->key, "_") != 0)
			arr[i++] = envp;
		envp = envp->next;
	}
}
