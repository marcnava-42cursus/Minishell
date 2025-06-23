/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:33:33 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/23 19:00:52 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

// Minishell execution builtins functions
int	msh_exec_bt_cd(const char *path);
int	msh_exec_bt_echo(void);
int	msh_exec_bt_env(const char **envp);
int	msh_exec_bt_exit(void);
int	msh_exec_bt_export(char *key, char *value);
int	msh_exec_bt_pwd(void);
int	msh_exec_bt_unset(void);

#endif