/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:58:38 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/26 19:12:08 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

/**
 * @brief Creates a new environment variable node.
 *
 * Allocates a t_envp node, duplicates key and value, and sets next to NULL.
 * If allocation fails or key/value is NULL, returns NULL and frees memory.
 *
 * @param key   Environment variable key string.
 * @param value Environment variable value string.
 * @return New node, or NULL on failure.
 */
t_envp	*envp_new_node(char *key, char *value)
{
	t_envp	*node;

	if (!key || !value)
		return (NULL);
	node = ft_calloc(1, sizeof(t_envp));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		ft_free((void **)&node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		ft_free((void **)&node->key);
		ft_free((void **)&node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

/**
 * @brief Appends a node at the end of the t_envp linked list.
 *
 * If node is NULL, the list is unchanged. If head is NULL, node becomes the
 * new head. Otherwise, traverses to the last node and links node as its next.
 *
 * @param head Pointer to the first element of the t_envp list.
 * @param node Node to append.
 * @return Pointer to the head of the modified list.
 */
t_envp	*envp_append_last(t_envp *head, t_envp *node)
{
	t_envp	*origin;

	if (!node)
		return (head);
	if (!head)
		return (node);
	origin = head;
	while (head->next)
		head = head->next;
	head->next = node;
	return (origin);
}

/**
 * @brief Deletes the node with matching key from a t_envp list.
 *
 * Searches for the first node with key equal to provided key. If found, it
 * is removed from the list, its key and value are freed, and memory freed.
 *
 * @param head Pointer to the head of the list.
 * @param key  Key of the node to delete.
 * @return Pointer to the head of the updated list.
 */
t_envp	*envp_del_key(t_envp *head, char *key)
{
	t_envp	*prev;
	t_envp	*current;

	if (!head || !key)
		return (head);
	prev = NULL;
	current = head;
	while (current && ft_strcmp(current->key, key))
	{
		prev = current;
		current = current->next;
	}
	if (!current)
		return (head);
	if (prev)
		prev->next = current->next;
	else
		head = current->next;
	ft_free((void **)&current->key);
	ft_free((void **)&current->value);
	ft_free((void **)&current);
	return (head);
}

/**
 * @brief Updates or adds an environment variable.
 *
 * Parses input "KEY=VALUE". If KEY exists in the list, its value is replaced.
 * Otherwise, a new node is appended with the given key and value.
 *
 * @param head Pointer to the head of the list.
 * @param key  String "KEY=VALUE" to set.
 * @return Pointer to the head of the modified list.
 */
t_envp	*envp_edit_key(t_envp *head, char *key)
{
	char	*eq, *var, *val; t_envp *current;

	if (!key || !(eq = ft_strchr(key, '=')) || eq == key)
		return (head);
	var = ft_substr(key, 0, eq - key);
	if (!var)
		return (head);
	val = ft_strdup(eq + 1);
	if (!val)
	{ ft_free((void **)&var); return (head); }
	current = head;
	while (current && ft_strcmp(current->key, var))
		current = current->next;
	if (current)
	{ ft_free((void **)&current->value); current->value = val;
	  ft_free((void **)&var); return (head); }
	return (envp_append_last(head, envp_new_node(var, val)));
}

/**
 * @brief Frees all nodes in a t_envp list.
 *
 * Iterates through the list, frees key and value strings, and nodes.
 * Returns NULL.
 *
 * @param head Pointer to the head of the list.
 */
void	envp_clear(t_envp *head)
{
	t_envp	*current;
	t_envp	*next;

	current = head;
	while (current)
	{
		next = current->next;
		ft_free((void **)&current->key);
		ft_free((void **)&current->value);
		ft_free((void **)&current);
		current = next;
	}
}
