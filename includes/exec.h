/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:42:35 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/06 20:50:11 by marcnava         ###   ########.fr       */
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

int		exec_tree(t_ent *tree, t_envp *envp, t_config *config);

// Módulos separados
int		exec_command(t_ent *node, t_envp **envp, t_config *config);
int		exec_pipeline(t_ent *node, t_envp **envp, t_config *config);
int		exec_logic(t_ent *node, t_envp **envp, t_config *config);
int		exec_subshell(t_ent *node, t_envp **envp, t_config *config);

// Utils internos
int		is_builtin(char *cmd);
int		execute_builtin(t_ent *node, t_envp **envp, t_config *config);
int		apply_redirections(t_ent *node);
void	close_fds(t_ent *node);
char	*find_command_path(char *cmd, t_envp *envp);
char	**envp_to_array(t_envp *envp);
void	free_env_array(char **env_array);

#endif
