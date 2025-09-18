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
#include "utils.h"

/* Prototipos de utilidades locales en exit_utils.c */
int		parse_ll_strict(const char *s, long long *out);
int		count_args(char **argv);
int		norm_exit_code_from_ll(long long v);

static void	print_exit_prefix_if_interactive(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
}

static void	exit_with_code_and_prefix(int code)
{
	print_exit_prefix_if_interactive();
	exit(code);
}

static void	numeric_error_and_exit(const char *arg)
{
	print_exit_prefix_if_interactive();
	print_err2("minishell: exit: ", (char *)arg, "");
	print_err2(": numeric argument required\n", NULL, NULL);
	fsync(STDERR_FILENO);
	exit(2);
}

int	msh_exec_bt_exit(char **argv, int last_exit_code)
{
	long long	val;
	int			argc;

	argc = count_args(argv);
	if (argc == 1)
		exit_with_code_and_prefix(last_exit_code);
	if (!parse_ll_strict(argv[1], &val))
		numeric_error_and_exit(argv[1]);
	if (argc > 2)
	{
		print_err2("minishell: exit: too many arguments\n", NULL, NULL);
		return (1);
	}
	exit_with_code_and_prefix(norm_exit_code_from_ll(val));
	return (0);
}
