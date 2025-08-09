/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:16:48 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/08 17:20:28 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"
# include "utils.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>

int		parse_command(t_envp *envp, t_config *config, t_ent **tree,
			char *command);
t_ent	*parse_command_tree(const char *cmd);
char	*expand_variables(const char *in, t_envp *envp, int exit_code);

/* Parser tree functions */
t_ent	*parse_list(const char **s);
t_ent	*parse_and(const char **s);
t_ent	*parse_pipeline(const char **s);
t_ent	*parse_primary(const char **s);

/* Parser utils functions */
t_ent	*parse_cmd(const char **s);
t_ent	*parse_subshell(const char **s);
int		handle_heredoc(const char *delimiter);

/* Environment functions */
t_envp	*save_envp(char **env);

#endif
