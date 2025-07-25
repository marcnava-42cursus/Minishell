/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:33:33 by marcnava          #+#    #+#             */
/*   Updated: 2025/07/03 16:59:22 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include "structs.h"

// Minishell execution builtins functions
int	msh_exec_bt_cd(const char *path);
int	msh_exec_bt_echo(t_ent *cmd);
int	msh_exec_bt_env(t_envp *envp);
int	msh_exec_bt_exit(void);
int	msh_exec_bt_export(t_envp **envp, char *key, char *value);
int	msh_exec_bt_pwd(void);
int	msh_exec_bt_unset(t_envp **envp, char *key);

#endif