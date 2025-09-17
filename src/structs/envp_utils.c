/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:42:14 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 18:42:14 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

/**
 * @brief Gets the value of an environment variable by key.
 *
 * Searches through the environment variable linked list for a node with
 * the specified key. If found, returns its value. If not found, returns
 * an empty string.
 *
 * @param envp Pointer to the head of the environment variable list.
 * @param key  Key of the environment variable to search for.
 * @return Value of the environment variable, or empty string if not found.
 */
char	*get_env_value(t_envp *envp, const char *key)
{
	while (envp)
	{
		if (!ft_strcmp(envp->key, (char *)key))
			return (envp->value);
		envp = envp->next;
	}
	return ("");
}
