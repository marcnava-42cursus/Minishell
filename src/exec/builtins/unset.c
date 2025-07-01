/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:50:37 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/26 19:36:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

/**
 * @brief Print invalid identifier error to stderr.
 *
 * Uses ft_putstr_fd to write the standard unset error message
 * format to file descriptor 2.
 *
 * @param key The invalid identifier string.
 * @return Always returns 1 to indicate failure.
 */
static int	print_id_error(char *key)
{
	ft_putstr_fd("msh: unset: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

/**
 * @brief Execute the unset builtin: remove an environment variable.
 *
 * Validates that key is a valid identifier ([A-Za-z_][A-Za-z0-9_]*).
 * If invalid, prints error to stderr and returns 1. Otherwise,
 * removes the variable (if present) and returns 0.
 *
 * @param envp Pointer to the head of the envp list (by reference).
 * @param key  The name of the variable to unset.
 * @return 0 on success, 1 on invalid identifier.
 */
int	msh_exec_bt_unset(t_envp **envp, char *key)
{
	size_t	i;
	t_envp	*new_head;

	if (!envp || !key)
		return (1);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (print_id_error(key));
	i = 0;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (print_id_error(key));
	}
	new_head = envp_del_key(*envp, key);
	*envp = new_head;
	return (0);
}
