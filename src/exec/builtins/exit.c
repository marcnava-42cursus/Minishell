/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:49:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 20:49:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"
#include <limits.h>
#include <unistd.h>

static int	parse_ll_strict(const char *s, long long *out)
{
	int	i;
	int	sign;
	ull	acc;
	ull	limit;
	int	d;

	if (!s || !*s)
		return (0);
	i = 0;
	sign = 1;
	if (s[i] == '+' || s[i] == '-')
		sign = (s[i++] == '-') ? -1 : 1;
	if (!s[i])
		return (0);
	acc = 0ULL;
	limit = (sign < 0) ? (ull)LLONG_MAX + 1ULL : (ull)LLONG_MAX;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		d = s[i] - '0';
		if (acc > (limit / 10ULL) || (acc == (limit / 10ULL)
				&& (ull)d > (limit % 10ULL)))
			return (0);
		acc = acc * 10ULL + (ull)d;
		i++;
	}
	if (sign > 0)
		*out = (long long)acc;
	else
	{
		if (acc == (ull)LLONG_MAX + 1ULL)
			*out = LLONG_MIN;
		else
			*out = -(long long)acc;
	}
	return (1);
}

static void	print_exit_prefix_if_interactive(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
}

int	msh_exec_bt_exit(char **argv, int last_exit_code)
{
	long long	val;
	int			argc;
	int			ecode;

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc == 1)
	{
		print_exit_prefix_if_interactive();
		exit(last_exit_code);
	}
	if (!parse_ll_strict(argv[1], &val))
	{
		print_exit_prefix_if_interactive();
		print_err2("minishell: exit: ", argv[1],
			": numeric argument required\n");
		fsync(STDERR_FILENO);
		exit(2);
	}
	if (argc > 2)
		return (print_err2("minishell: exit: too many arguments\n",
				NULL, NULL), 1);
	ecode = (int)(val % 256);
	if (ecode < 0)
		ecode += 256;
	print_exit_prefix_if_interactive();
	exit(ecode);
}
