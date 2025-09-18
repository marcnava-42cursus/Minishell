/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:03:25 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/19 00:03:25 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	count_env_entries(t_envp *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		count++;
		envp = envp->next;
	}
	return (count);
}

static int	fill_env_array(t_envp *envp, char **env_array, int count)
{
	char	*temp;
	int		i;

	i = 0;
	while (envp && i < count)
	{
		temp = ft_strjoin(envp->key, "=");
		if (!temp)
			return (1);
		if (envp->value)
			env_array[i] = ft_strjoin(temp, envp->value);
		else
			env_array[i] = ft_strjoin(temp, "");
		ft_free((void **)&temp);
		if (!env_array[i])
			return (1);
		envp = envp->next;
		i++;
	}
	return (0);
}

char	**envp_to_array(t_envp *envp)
{
	char	**env_array;
	int		count;

	if (!envp)
		return (NULL);
	count = count_env_entries(envp);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (fill_env_array(envp, env_array, count))
		return (ft_free_matrix((void **)env_array), NULL);
	env_array[count] = NULL;
	return (env_array);
}
