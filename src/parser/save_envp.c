/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:49:19 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/31 01:02:08 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

static t_envp	*update_env_node(t_envp *node, char *env_var)
{
	char	*equal;
	size_t	key_len;
	size_t	i;

	equal = ft_strchr(env_var, '=');
	if (!equal)
		return (NULL);
	key_len = (size_t)(equal - env_var);
	node->key = ft_calloc(key_len + 1, sizeof(char));
	if (!node->key)
		return (ft_free((void **)&node), NULL);
	i = 0;
	while (i < key_len)
	{
		node->key[i] = env_var[i];
		i++;
	}
	node->value = ft_strdup(equal + 1);
	if (!node->value)
		return (ft_free((void **)&node->key),
		        ft_free((void **)&node), NULL);
	node->next = NULL;
	return (node);
}

t_envp	*save_envp(char **env)
{
	t_envp	*head;
	t_envp	*node;

	if (!env || !*env)
	{
		printf("No env provided\n");
		return (NULL);
	}
	head = NULL;
	while (*env)
	{
		node = ft_calloc(1, sizeof(t_envp));
		if (!node)
			return (envp_clear(head), NULL);
		if (!update_env_node(node, *env))
			return (envp_clear(head), NULL);
		head = envp_append_last(head, node);
		env++;
	}
	return (head);
}
