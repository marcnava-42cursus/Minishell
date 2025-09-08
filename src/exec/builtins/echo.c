/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:29:31 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/03 17:03:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

static int	check_flag(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == 'n')
			i++;
		else
			return (0);
	}
	return (1);
}

int	msh_exec_bt_echo(t_ent *cmd)
{
	int	i;
	int	newline;
	int	check;

	i = 1;
	newline = 1;
	check = 1;
	while (cmd->argv[i] && !ft_strncmp(cmd->argv[i], "-n", 2) && check)
	{
		check = check_flag(cmd->argv[i]);
		if (i == 1 && check)
			newline = 0;
		i++;
	}
	if (ft_strcmp(cmd->argv[i - 1], "echo") && !check)
		i--;
	while (cmd->argv[i++])
    {
        printf("%s", cmd->argv[i - 1]);
        if (cmd->argv[i])
            printf(" ");
    }
	if (newline)
		printf("\n");
	return (0);
}
