/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:07:39 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:41:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <termios.h>
# include <unistd.h>
# include "structs.h"

void	print_tree(t_ent *node, int indent);
void	print_err2(const char *a, const char *b, const char *c);

void	enable_raw(struct termios *terminal_attrs);
void	disable_raw(struct termios *terminal_attrs);
void	clear_scr(void);

char	**ft_realloc_matrix(char **old, int oldc, char *new_str);

// Minishell
int		ms_init(t_mshell *ms, char **env);
void	ms_cleanup(t_mshell *ms);
void	ms_setup_suggestions(t_mshell *ms);
char	*read_line_dispatch(t_mshell *ms);
char	*read_line_tty(const t_mshell *ms);
char	*read_line_nontty(void);
void	process_line(t_mshell *ms, char *line);
void	refresh_prompt_and_sugg(t_mshell *ms);
int		check_args_env(int argc, char **argv, char **env);
void	apply_signal_exit_code(t_mshell *ms);
int		parse_and_maybe_exec(t_mshell *ms, char *line);

#endif