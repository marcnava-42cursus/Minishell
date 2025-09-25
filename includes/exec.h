/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:42:35 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:13:15 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include "structs.h"
# include "exec_builtins.h"
# include "signals.h"

/* t_pipe_ctx se declara ahora en structs.h */

// Punto de entrada principal (exec.c)
int		exec_tree(t_mshell *mshell);

// Funciones de ejecución de comandos (exec_commands.c)
int		exec_command(t_ent *node, t_mshell *mshell);
int		exec_pipeline(t_ent *node, t_mshell *mshell);
int		exec_logic(t_ent *node, t_mshell *mshell);
int		exec_subshell(t_ent *node, t_mshell *mshell);
int		exec_builtin(t_ent *node, t_mshell *mshell);

// Utilidades básicas (exec_utils.c)
int		is_builtin(char *cmd);
int		apply_redirections(t_ent *node);

// Funciones auxiliares (exec_helpers.c)
char	*find_command_path(char *cmd, t_envp *envp);
char	**process_argv_quotes(char **argv);
char	**envp_to_array(t_envp *envp);

// Utilidades específicas de comandos (exec_command_utils.c)
void	handle_child_process(t_ent *node, t_mshell *mshell, char **env_arr);
int		wait_for_child_and_cleanup(pid_t pid, t_mshell *mshell, char **env_arr);

// Utilidades específicas de pipelines (exec_pipeline_utils.c)
int		count_pipeline_commands(t_ent *node);
void	collect_pipeline_commands(t_ent *node, t_ent **commands, int cmd_count);
int		setup_pipeline_pipes(int **pipes, int cmd_count);
void	setup_input_redirection(int **pipes, t_ent *command, int i);
void	setup_output_redirection(int **pipes, t_ent *command, int i,
			int cmd_count);

// Helpers de ejecución de pipeline (exec_pipeline_run.c)
int		single_command_from_pipeline(t_ent *node, t_mshell *mshell);
int		spawn_children(t_pipe_ctx *ctx);
void	wait_children(const t_pipe_ctx *ctx, int *last_status,
			int *first_sigpipe);
void	free_pipe_rows(int **pipes, int cmd_count);
int		should_print_broken_pipe(const t_pipe_ctx *ctx, int first_sigpipe);
int		status_to_exitcode(int status);
void	free_ctx_arrays(t_pipe_ctx *ctx);
void	exec_pipeline_child(t_pipe_ctx *ctx, int i);

// Accesores externos usados en varias unidades
int		exec_subshell(t_ent *node, t_mshell *mshell);

// Utilidades específicas de builtins (exec_builtin_utils.c)
int		handle_cd_builtin(char **processed_argv, t_envp **envp);
int		handle_echo_builtin(t_ent *node, char **processed_argv);
int		handle_export_builtin(char **processed_argv, t_envp **envp);
int		handle_unset_builtin(char **processed_argv, t_envp **envp);
int		handle_exit_builtin(char **processed_argv, int last_exit_code);

#endif
