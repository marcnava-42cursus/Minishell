/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 03:09:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 03:09:30 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	exp_dollar(const char **in, char **out,
	t_envp *envp, int exit_code)
{
	char	*var;
	char	*val;
	char	*num;

	if (*(*in + 1) == '?')
	{
		num = ft_itoa(exit_code);
		append_str(out, num);
		ft_free((void **)&num);
		*in += 2;
		return ;
	}
	var = read_var(in);
	val = get_env_value(envp, var);
	if (!val)
		val = "";
	append_str(out, val);
	if (var)
		ft_free((void **)&var);
}
