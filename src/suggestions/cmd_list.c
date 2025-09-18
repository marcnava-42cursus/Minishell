/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:35:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 11:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "suggestions.h"

void	cmdlist_init(t_cmd_list *list)
{
	if (!list)
		return ;
	list->items = NULL;
	list->count = 0;
	list->capacity = 0;
}

static void	cmdlist_resize(t_cmd_list *list)
{
	size_t	new_capacity;
	char	**new_items;
	size_t	i;

	if (!list)
		return ;
	if (list->capacity == 0)
		new_capacity = CMD_INITIAL_CAPACITY;
	else
		new_capacity = list->capacity * 2;
	new_items = malloc(new_capacity * sizeof(char *));
	if (!new_items)
		return ;
	i = 0;
	while (i < list->count)
	{
		new_items[i] = list->items[i];
		i++;
	}
	ft_free((void **)&list->items);
	list->items = new_items;
	list->capacity = new_capacity;
}

void	cmdlist_add(t_cmd_list *list, const char *name)
{
	if (!list || !name)
		return ;
	if (list->count == list->capacity)
		cmdlist_resize(list);
	if (list->count < list->capacity)
	{
		list->items[list->count] = sug_strdup(name);
		if (list->items[list->count])
			list->count++;
	}
}

const char	*cmdlist_find_suggestion(t_cmd_list *list, const char *prefix)
{
	size_t	i;

	if (!list || !prefix || !prefix[0])
		return (NULL);
	i = 0;
	while (i < list->count)
	{
		if (sug_is_prefix(list->items[i], prefix))
			return (list->items[i]);
		i++;
	}
	return (NULL);
}

void	cmdlist_free(t_cmd_list *list)
{
	size_t	i;

	if (!list)
		return ;
	i = 0;
	while (i < list->count)
	{
		ft_free((void **)&list->items[i]);
		i++;
	}
	ft_free((void **)&list->items);
	list->items = NULL;
	list->count = 0;
	list->capacity = 0;
}
