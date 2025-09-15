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
#include <limits.h>
#include <unistd.h>

static int	parse_ll_strict(const char *s, long long *out)
{
	int			i;
	int			sign;
	unsigned long long acc;
	unsigned long long limit;
	int			d;

	if (!s || !*s)
		return (0);
	i = 0;
	sign = 1;
	if (s[i] == '+' || s[i] == '-')
		sign = (s[i++] == '-') ? -1 : 1;
	if (!s[i])
		return (0);
	acc = 0ULL;
	limit = (sign < 0) ? (unsigned long long)LLONG_MAX + 1ULL
					  : (unsigned long long)LLONG_MAX;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		d = s[i] - '0';
		if (acc > (limit / 10ULL) || (acc == (limit / 10ULL)
			&& (unsigned long long)d > (limit % 10ULL)))
			return (0);
		acc = acc * 10ULL + (unsigned long long)d;
		i++;
	}
	if (sign > 0)
		*out = (long long)acc;
	else
	{
		if (acc == (unsigned long long)LLONG_MAX + 1ULL)
			*out = LLONG_MIN;
		else
			*out = -(long long)acc;
	}
	return (1);
}

static void	print_exit_prefix_if_interactive(void)
{
	/* Print the banner only in interactive mode (like bash). */
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
}

int	msh_exec_bt_exit(char **argv, int last_exit_code)
{
	long long	val;
	int			argc;
	int			ecode;

	/* Count arguments */
	argc = 0;
	while (argv[argc])
		argc++;

	/* No arguments: exit with last exit code */
	if (argc == 1)
	{
		print_exit_prefix_if_interactive();
		exit(last_exit_code);
	}
	
	/* Validate first argument */
	if (!parse_ll_strict(argv[1], &val))
	{
		print_exit_prefix_if_interactive();
		print_err2("minishell: exit: ", argv[1], ": numeric argument required\n");
		/* Ensure message is flushed to stderr before exiting */
		fsync(STDERR_FILENO);
		exit(2);
	}
	/* Too many arguments: error but don't exit */
	if (argc > 2)
	{
		print_err2("minishell: exit: too many arguments\n", NULL, NULL);
		return (1);
	}
	/* Compute code modulo 256 like bash */
	ecode = (int)(val % 256);
	if (ecode < 0)
		ecode += 256;
	print_exit_prefix_if_interactive();
	exit(ecode);
}
