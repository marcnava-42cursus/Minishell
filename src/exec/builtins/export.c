/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:44:36 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 20:36:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

int	msh_exec_bt_export_print(t_envp *envp)
{
	t_envp	*cur;

	cur = envp;
	while (cur)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(cur->key, 1);
		if (cur->value && cur->value[0] != '\0')
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(cur->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		cur = cur->next;
	}
	return (0);
}

/**
 * @brief Print invalid identifier error for export builtin.
 * @param key The invalid identifier string.
 * @return Always returns 1 to indicate error.
 */
static int	print_id_error_export(char *key)
{
	ft_putstr_fd("msh: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

/**
 * @brief Execute the export builtin: add or update an env var.
 * @param envp Pointer to head of envp list (by reference).
 * @param key  Env var name.
 * @param value Env var value; if NULL, empty string used.
 * @return 0 on success, 1 on invalid identifier.
 */
int	msh_exec_bt_export(t_envp **envp, char *key, char *value)
{
	t_envp	*current;
	char	*p;

	if (!envp || !key)
		return (1);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (print_id_error_export(key));
	p = key + 1;
	while (*p)
	{
		if (!ft_isalnum(*p) && *p != '_')
			return (print_id_error_export(key));
		p++;
	}
	if (!value)
		value = "";
	current = *envp;
	while (current && ft_strcmp(current->key, key))
		current = current->next;
	if (current)
		(ft_free((void **)&current->value), current->value = ft_strdup(value));
	else
		*envp = envp_append_last(*envp, envp_new_node(key, value));
	return (0);
}
