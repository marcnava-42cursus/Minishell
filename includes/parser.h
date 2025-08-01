/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:16:48 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/01 18:45:27 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

t_envp	*save_envp(char **env);

int		parse_command(t_envp *envp, t_ent **tree, char *command);

char	*expand_variables(const char *in, t_envp *envp);

#endif