/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:33:33 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:50:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_BUILTINS_H
# define EXEC_BUILTINS_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "libft.h"
# include "structs.h"

typedef unsigned long long	t_ull;

// Helper functions for environment management
char	*envp_get_value(t_envp *envp, const char *key);
int		envp_set_value(t_envp **envp, const char *key, const char *value);

// Minishell execution builtins functions
int		msh_exec_bt_cd(t_envp **envp, const char *path);
int		msh_exec_bt_echo(t_ent *cmd);
int		msh_exec_bt_env(t_envp *envp);
int		msh_exec_bt_exit(char **argv, int last_exit_code);
int		msh_exec_bt_export(t_envp **envp, char *key, char *value);
int		msh_exec_bt_export_print(t_envp *envp);
int		msh_exec_bt_pwd(t_envp *envp);
int		msh_exec_bt_unset(t_envp **envp, char *key);

#endif