/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:35:22 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 23:35:22 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "utils.h"

void	exit_with_errno_message(const char *cmd)
{
	print_err2("minishell: ", cmd, ": ");
	print_err2(strerror(errno), "\n", NULL);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}
