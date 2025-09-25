/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_underscore.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 00:24:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/26 00:26:28 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ms_update_underscore(t_mshell *mshell, char **argv)
{
	char	**unquoted;
	int		count;

	if (!mshell || !argv)
		return ;
	count = 0;
	while (argv[count])
		count++;
	if (count == 0)
		return ;
	unquoted = process_argv_quotes(argv);
	if (!unquoted)
		return ;
	if (unquoted[count - 1])
		envp_set_value(&(mshell->envp), "_", unquoted[count - 1]);
	ft_free_matrix((void **)unquoted);
}

void	ms_update_underscore_after_exec(t_mshell *mshell)
{
	char	*val;

	if (!mshell || !mshell->tree)
		return ;
	val = compute_underscore_value(mshell->tree);
	if (!val)
		return ;
	envp_set_value(&(mshell->envp), "_", val);
	ft_free((void **)&val);
}

char	*compute_underscore_value(t_ent *root)
{
	int		count;
	t_ent	**cmds;
	char	*val;

	if (!root)
		return (NULL);
	count = count_pipeline_commands(root);
	if (count <= 0)
		return (NULL);
	if (count == 1)
	{
		val = dup_last_export_assign(root->argv);
		if (val)
			return (val);
		return (dup_last_arg(root->argv));
	}
	cmds = malloc(sizeof(t_ent *) * count);
	if (!cmds)
		return (NULL);
	collect_pipeline_commands(root, cmds, count);
	val = dup_from_pipeline_commands(cmds, count);
	ft_free((void **)&cmds);
	return (val);
}
