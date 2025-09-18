/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:42:35 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 04:24:13 by marcnava         ###   ########.fr       */
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
# include "structs.h"
# include "exec_builtins.h"
# include "signals.h"

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
int		wait_for_child_and_cleanup(pid_t pid, char **env_arr);

// Utilidades específicas de pipelines (exec_pipeline_utils.c)
void	perror_exit(char *msg, int exit_code);
int		count_pipeline_commands(t_ent *node);
void	collect_pipeline_commands(t_ent *node, t_ent **commands, int cmd_count);
int		setup_pipeline_pipes(int **pipes, int cmd_count);
void	setup_input_redirection(int **pipes, t_ent *command, int i);
void	setup_output_redirection(int **pipes, t_ent *command, int i,
			int cmd_count);
void	close_all_pipes(int **pipes, int cmd_count);
void	cleanup_pipeline_resources(t_ent **commands, int **pipes, pid_t *pids,
			int cmd_count);

// Utilidades específicas de builtins (exec_builtin_utils.c)
int		handle_cd_builtin(char **processed_argv, t_envp **envp);
int		handle_echo_builtin(t_ent *node, char **processed_argv);
int		handle_export_builtin(char **processed_argv, t_envp **envp);
int		handle_unset_builtin(char **processed_argv, t_envp **envp);
int		handle_exit_builtin(char **processed_argv, int last_exit_code);

#endif
