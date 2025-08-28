/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:49:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/26 15:56:26 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

static int	ft_is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	msh_exec_bt_exit(char **argv, int last_exit_code)
{
	int	exit_code;
	int	argc;

	/* Count arguments */
	argc = 0;
	while (argv[argc])
		argc++;
	
	printf("exit\n");
	
	/* No arguments: exit with last exit code */
	if (argc == 1)
		exit(last_exit_code);
	
	/* More than one argument: error but don't exit */
	if (argc > 2)
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	
	/* One argument: validate and use as exit code */
	if (!ft_is_valid_number(argv[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		exit(2);
	}
	
	exit_code = ft_atoi(argv[1]);
	/* Exit code is taken modulo 256 (bash behavior) */
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	
	exit(exit_code);
}
