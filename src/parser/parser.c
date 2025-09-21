/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:17 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:32:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

int	parse_command(t_mshell *mshell, char *cmd)
{
	if (!mshell || !cmd)
		return (1);
	return (pm_run(mshell, cmd));
}
