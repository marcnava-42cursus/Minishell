/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:35:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/29 15:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

/**
 * @brief Get the value of an environment variable from the envp list.
 * 
 * @param envp The environment variables list.
 * @param key The key to search for.
 * @return The value string if found, NULL otherwise.
 */
char	*envp_get_value(t_envp *envp, const char *key)
{
	t_envp	*current;

	if (!envp || !key)
		return (NULL);
	current = envp;
	while (current)
	{
		if (ft_strcmp(current->key, (char *)key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Set or update an environment variable in the envp list.
 * 
 * @param envp Pointer to the environment variables list.
 * @param key The key to set.
 * @param value The value to set.
 * @return 0 on success, 1 on failure.
 */
int	envp_set_value(t_envp **envp, const char *key, const char *value)
{
	t_envp	*current;

	if (!envp || !key || !value)
		return (1);
	current = *envp;
	while (current)
	{
		if (ft_strcmp(current->key, (char *)key) == 0)
		{
			ft_free((void **)&current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (1);
			return (0);
		}
		current = current->next;
	}
	*envp = envp_append_last(*envp, envp_new_node((char *)key, (char *)value));
	return (0);
}
