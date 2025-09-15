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
 * @brief Execute the unset builtin: remove an environment variable.
 *
 * Bash behavior reference:
 * - Returns 0 whether the variable exists or not.
 * - Does not error on names that are not valid identifiers (they are ignored).
 * - Options like -v/-f are not supported in this minishell; arguments starting
 *   with '-' are treated as names and do not cause an error.
 *
 * @param envp Pointer to the head of the envp list (by reference).
 * @param key  The name of the variable to unset.
 * @return Always 0.
 */
int	msh_exec_bt_unset(t_envp **envp, char *key)
{
	t_envp	*new_head;

	if (!envp || !key)
		return (0);
	new_head = envp_del_key(*envp, key);
	*envp = new_head;
	return (0);
}
