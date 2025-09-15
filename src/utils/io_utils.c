/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 22:51:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/08 22:51:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "libft.h"
#include <unistd.h>

void	print_err2(const char *a, const char *b, const char *c)
{
	if (a)
		write(STDERR_FILENO, a, ft_strlen(a));
	if (b)
		write(STDERR_FILENO, b, ft_strlen(b));
	if (c)
		write(STDERR_FILENO, c, ft_strlen(c));
}

