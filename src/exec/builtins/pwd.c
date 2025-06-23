/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:37:47 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/23 15:48:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtins.h"

int	msh_exec_bt_pwd(void)
{
	char	*pwd;

	pwd = getenv("PWD");
	if (!pwd)
		return (1);
	printf("%s\n", pwd);
	return (0);
}
