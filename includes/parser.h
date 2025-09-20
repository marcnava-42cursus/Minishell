/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:16:48 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 04:07:35 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"
# include "utils.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>

int		parse_command(t_mshell *mshell, char *command);
t_ent	*parse_command_tree(const char *cmd, t_mshell *mshell);
char	*expand_variables(const char *in, t_envp *envp, int exit_code);

/* Parser tree functions */
t_ent	*parse_list(const char **s, t_mshell *mshell);
t_ent	*parse_and(const char **s, t_mshell *mshell);
t_ent	*parse_pipeline(const char **s, t_mshell *mshell);
t_ent	*parse_primary(const char **s, t_mshell *mshell);

/* Parser utils functions */
t_ent	*parse_cmd(const char **s, t_mshell *mshell);
t_ent	*parse_subshell(const char **s, t_mshell *mshell);
int		handle_heredoc(const char *delimiter, t_mshell *mshell);
int		check_heredoc_quotes(const char *cmd, const char *delimiter_pos);
char	*preprocess_heredocs(const char *cmd, t_mshell *mshell);

/* Environment functions */
t_envp	*save_envp(char **env);

#endif
