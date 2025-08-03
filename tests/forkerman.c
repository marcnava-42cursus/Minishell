/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkerman.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:11:19 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 13:28:55 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forkerman.h"

int main(void)
{
	printf("ASCII FORKERMAN - Puzzle with exit\n");
	printf("WASD=move SPACE=bomb R=restart Q=quit\n");
	printf("Press any key to start...\n");
	read(0, &(char){0}, 1);
	forkerman();
	exit(0);
}
