/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:01:01 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/30 21:01:03 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include "libft.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_HEREDOC,
	NODE_INPUT,
	NODE_OUTPUT,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_ent
{
	t_node_type		type;
	char			**argv;
	int				fd_in;
	int				fd_out;
	struct s_ent	*child;
	struct s_ent	*next;
}					t_ent;

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}					t_envp;

typedef struct s_config
{
	char	*prompt_raw;
	char	*prompt;
	int		exit_code;
}			t_config;

t_envp	*envp_new_node(char *key, char *value);
t_envp	*envp_append_last(t_envp *head, t_envp *node);
t_envp	*envp_del_key(t_envp *head, char *key);
t_envp	*envp_edit_key(t_envp *head, char *key);
void	envp_clear(t_envp *head);

t_ent	*ent_new_node(t_node_type type, char **argv);
int		ent_new_sibling(t_ent *node, t_ent *sibling);
int		ent_new_child(t_ent *parent, t_ent *child);
void	ent_free_node(t_ent *node);
void	ent_free(t_ent *root);

#endif