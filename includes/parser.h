/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:16:48 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:05:11 by marcnava         ###   ########.fr       */
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

int			parse_command(t_mshell *mshell, char *command);

/* Expansion */
t_ent		*parse_command_tree(const char *cmd, t_mshell *mshell);
char		*exp_expand(const char *in, t_envp *envp, int exit_code);

/* Expansion helpers (split across files) */
void		append_str(char **dst, const char *src);
void		append_char(char **dst, char c);
char		*read_var(const char **p);
void		expand_in_dquotes(const char **p, char **out,
				t_envp *envp, int exit_code);
void		exp_dollar(const char **in, char **out, t_envp *envp,
				int exit_code);
int			exp_process_char(const char **in, char **out,
				t_envp *envp, int exit_code);

/* Parser main helpers */
int			pm_store_raw_command(t_mshell *mshell, const char *cmd);
char		*pm_expand_cmd(const char *cmd, t_mshell *mshell);
int			pm_is_empty_or_whitespace(const char *str);
int			pm_store_expanded(t_mshell *mshell, const char *exp);
t_ent		*pm_parse_tree_or_error(const char *exp, t_mshell *mshell);
int			pm_prepare(t_mshell *mshell, const char *cmd, char **out_exp);
int			pm_parse_store(t_mshell *mshell, char *exp);
int			pm_run(t_mshell *mshell, const char *cmd);

/* Common parser helpers */
void		parser_skip_whitespace(const char **s);
char		*get_next_token(const char **s);

/* Tokenizer internals (exposed to split files) */
char		*tkn_return_operator(const char **s);
void		tkn_init_ctx(t_tokctx *c, const char *s);
void		tkn_push(t_tokctx *c, char ch);
void		tkn_loop(t_tokctx *c);
char		*tkn_read_word(const char **s);

/* Parser tree functions */
t_ent		*parse_list(const char **s, t_mshell *mshell);
t_ent		*parse_and(const char **s, t_mshell *mshell);
t_ent		*parse_pipeline(const char **s, t_mshell *mshell);
t_ent		*parse_primary(const char **s, t_mshell *mshell);

/* Parser tree helper functions */
void		advance_to_last(t_ent **node);
void		handle_parse_error(t_ent *node, t_mshell *mshell);
t_ent		*handle_pipe_parsing(const char **s, t_mshell *mshell,
				t_ent *head, t_ent *last);
t_ent		*handle_and_parsing(const char **s, t_mshell *mshell,
				t_ent *left);
t_ent		*handle_or_parsing(const char **s, t_mshell *mshell, t_ent *left);

/* Parser utils functions */
t_ent		*parse_cmd(const char **s, t_mshell *mshell);

/* Parser cmd helpers */
int			pc_handle_heredoc(const char **s, t_mshell *mshell, t_pc_ctx *ctx);
int			pc_handle_redir_in(const char **s, t_mshell *mshell, t_pc_ctx *ctx);
int			pc_handle_redir_out(const char **s, t_mshell *mshell, t_pc_ctx *ctx,
				int append);
int			pc_redir_blocked(t_pc_ctx *ctx);
int			pc_consume_filename(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx);
char		*pc_read_path(const char **s, t_mshell *mshell, t_pc_ctx *ctx);
void		pc_open_error(t_mshell *mshell, int *ctx_fd_target,
				const char *path);
int			pc_handle_word_or_glob(char *tok, t_pc_ctx *ctx);
t_ent		*pc_finish(const t_pc_ctx *ctx);
void		pc_close_replace(int *dst_fd, int new_fd);
char		*pc_read_filename_or_error(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx);
char		*pc_unquote(const char *str);
int			pc_init_ctx(t_pc_ctx *ctx);
int			pc_apply_token(const char **s, t_mshell *mshell,
				t_pc_ctx *ctx, char *tok);
void		copy_squoted(const char *s, size_t *i, char *out, size_t *j);
void		copy_dquoted(const char *s, size_t *i, char *out, size_t *j);

/* Parser main */
int			pm_run(t_mshell *mshell, const char *cmd);

/* Subshell and heredoc */
t_ent		*parse_subshell(const char **s, t_mshell *mshell);
int			handle_heredoc(const char *delimiter, t_mshell *mshell);
int			check_heredoc_quotes(const char *cmd, const char *delimiter_pos);
const char	*hd_after_arrow(const char *cmd);
char		*hd_extract_delim(const char *pos);
char		*hd_clean_delimiter(const char *delimiter, int *is_quoted);
int			hd_loop_write(int tmp_fd, const char *clean_delim,
				t_mshell *mshell, int is_quoted);
int			hd_prepare_ctx(t_hd_ctx *c, const char *delimiter);

/* Environment functions */
t_envp		*save_envp(char **env);

#endif
