/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suggestions.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:22:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/25 21:41:03 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUGGESTIONS_H
# define SUGGESTIONS_H

# include <unistd.h>
# include <dirent.h>
# include <termios.h>
# include <term.h>
# include <sys/stat.h>
# include "structs.h"
# include "utils.h"

/* String utility functions */
size_t				sug_strlen(const char *s);
int					sug_is_prefix(const char *str, const char *prefix);
char				*sug_strdup(const char *s);
void				sug_strcpy(char *dest, const char *src);
void				sug_strcat(char *dest, const char *src);

/* Command list functions */
void				cmdlist_init(t_cmd_list *list);
void				cmdlist_add(t_cmd_list *list, const char *name);
const char			*cmdlist_find_suggestion(t_cmd_list *list,
						const char *prefix);
void				cmdlist_free(t_cmd_list *list);
void				cmdlist_load_from_path(t_cmd_list *list, char *path_env);

/* Terminal functions */
int					terminal_init_caps(t_terminal *term);
int					terminal_putchar(int c);

/* Main suggestion functions */
t_suggestion_ctx	*suggestion_init(const char *prompt);
char				*suggestion_readline(t_suggestion_ctx *ctx);
void				suggestion_update_prompt(t_suggestion_ctx *ctx,
						const char *new_prompt);
void				suggestion_cleanup(t_suggestion_ctx *ctx);

/* Handlers */
char				*handle_enter(t_suggestion_ctx *ctx, char *buffer,
						size_t buffer_len, struct termios *original_termios);
int					handle_action_code(t_suggestion_ctx *ctx,
						t_readline_state *state, int action, char **out);
char				*handle_read_error(t_readline_state *state);

#endif
