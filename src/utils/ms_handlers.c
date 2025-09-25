/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 21:33:50 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:44:26 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	init_shlvl(t_envp **envp)
{
	const char	*value;
	int		level;
	char		*new_value;

	if (!envp)
		return ;
	value = envp_get_value(*envp, "SHLVL");
	if (!value)
	{
		envp_set_value(envp, "SHLVL", "1");
		return ;
	}
	level = ft_atoi(value);
	if (level < 0)
		level = 0;
	new_value = ft_itoa(level + 1);
	if (!new_value)
		return ;
	if (envp_set_value(envp, "SHLVL", new_value) != 0)
	{
		ft_free((void **)&new_value);
		return ;
	}
	ft_free((void **)&new_value);
}

int	ms_init(t_mshell *ms, char **env)
{
	ft_memset(ms, 0, sizeof(t_mshell));
	ms->config = malloc(sizeof(t_config));
	if (!ms->config)
		return (printf("Error allocating config\n"), 1);
	load_config(ms->config, env);
	ms->envp = save_envp(env);
	if (!ms->envp)
		return (printf("Error saving envp\n"), 1);
	init_shlvl(&ms->envp);
	ms->exit_code = 0;
	if (ms->config->prompt)
	{
		ft_free((void **)&ms->config->prompt);
		ms->config->prompt = build_prompt(ms->config->prompt_raw,
				ms->exit_code, ms->envp);
	}
	ms->should_exit = 0;
	setup_parent_signals();
	return (0);
}

void	ms_cleanup(t_mshell *ms)
{
	if (ms->raw_command)
		ft_free((void **)&ms->raw_command);
	if (ms->expanded_command)
		ft_free((void **)&ms->expanded_command);
	if (ms->tree)
		ent_free(ms->tree);
	if (ms->config)
	{
		if (ms->config->prompt)
			ft_free((void **)&ms->config->prompt);
		if (ms->config->prompt_raw)
			ft_free((void **)&ms->config->prompt_raw);
		ft_free((void **)&ms->config);
	}
	if (ms->envp)
		envp_clear(ms->envp);
	if (ms->suggestions)
		suggestion_cleanup(ms->suggestions);
}

void	ms_setup_suggestions(t_mshell *ms)
{
	if (!ms->config->use_suggestions)
	{
		ms->suggestions = NULL;
		return ;
	}
	ms->suggestions = suggestion_init(ms->config->prompt);
	if (!ms->suggestions)
		ms->config->use_suggestions = 0;
}
