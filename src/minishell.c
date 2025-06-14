/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:46:11 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/14 18:03:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	if (argc != 1)
		printf("Usage: %s\n", argv[0]);
	if (!env || !*env)
		printf("Error: Can't load env variables");
	return (0);
}
