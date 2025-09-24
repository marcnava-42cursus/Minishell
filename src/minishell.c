/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/24 23:15:34 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int	check_args_env(int argc, char **argv, char **env)
{
	if (argc != 1)
		return (printf("Usage: %s\n", argv[0]), 1);
	if (!env || !*env)
		return (printf("Error: Can't load env variables\n"), 1);
	return (0);
}

void	apply_signal_exit_code(t_mshell *ms)
{
	int	code;

	code = check_signal_exit_code();
	if (code != 0)
		ms->exit_code = code;
}

int	parse_and_maybe_exec(t_mshell *ms, char *line)
{
	if (parse_command(ms, line) != 0)
		return (1);
	if (ms->tree)
	{
		ms->exit_code = exec_tree(ms);
		if (ms->exit_code != 0 && isatty(STDIN_FILENO))
			printf("Command exited with code: %d\n", ms->exit_code);
		ent_free(ms->tree);
		ms->tree = NULL;
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_mshell	ms;

	if (check_args_env(argc, argv, env) != 0)
		return (1);
	if (ms_init(&ms, env) != 0)
		return (1);
	ms_setup_suggestions(&ms);
	while (!ms.should_exit)
	{
		line = read_line_dispatch(&ms);
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		apply_signal_exit_code(&ms);
		if (*line)
			process_line(&ms, line);
		ft_free((void **)&line);
	}
	ms_cleanup(&ms);
	return (ms.exit_code);
}
