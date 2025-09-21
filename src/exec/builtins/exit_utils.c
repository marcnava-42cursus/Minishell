/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:23:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/19 00:23:12 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

int	count_args(char **argv)
{
	int	c;

	c = 0;
	while (argv[c])
		c++;
	return (c);
}

int	norm_exit_code_from_ll(long long v)
{
	int	code;

	code = (int)(v % 256);
	if (code < 0)
		code += 256;
	return (code);
}
