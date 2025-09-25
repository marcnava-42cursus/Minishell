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

static int	count_visible(t_envp *envp)
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

static void	fill_ptrs(t_envp *envp, t_envp **arr)
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

static void	sort_by_key(t_envp **arr, int n)
{
	int		i;
	int		j;
	t_envp	*tmp;

	i = 1;
	while (i < n)
	{
		j = i;
		while (j > 0 && ft_strcmp(arr[j - 1]->key, arr[j]->key) > 0)
		{
			tmp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = tmp;
			j--;
		}
		i++;
	}
}

static void	print_sorted_entries(t_envp **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(arr[i]->key, 1);
		if (arr[i]->value && arr[i]->value[0] != '\0')
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(arr[i]->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		i++;
	}
}

int	msh_exec_bt_export_print(t_envp *envp)
{
	int		count;
	t_envp	**arr;

	count = count_visible(envp);
	if (count <= 0)
		return (0);
	arr = malloc(sizeof(t_envp *) * count);
	if (!arr)
		return (1);
	fill_ptrs(envp, arr);
	sort_by_key(arr, count);
	print_sorted_entries(arr, count);
	free(arr);
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
