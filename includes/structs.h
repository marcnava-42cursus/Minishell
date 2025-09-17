/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:01:01 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/17 20:38:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include "libft.h"

# define SUG_BUFFER_SIZE 1024
# define CMD_INITIAL_CAPACITY 256

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_ent
{
	t_node_type		type;
	char			**argv;
	int				fd_in;
	int				fd_out;
	int				exit_code;
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
	int		use_suggestions;
	char	*prompt_raw;
	char	*prompt;
}			t_config;

/**
 * @brief Dynamic list for storing command suggestions
 */
typedef struct s_cmd_list
{
	char	**items;
	size_t	count;
	size_t	capacity;
}			t_cmd_list;

/**
 * @brief Terminal capabilities for suggestions
 */
typedef struct s_terminal
{
	int				capabilities_loaded;
	char			*clear_to_eol;
	char			*cursor_move;
}					t_terminal;

/**
 * @brief Suggestion context containing all necessary data
 */
typedef struct s_suggestion_ctx
{
	t_cmd_list	*commands;
	t_terminal	*terminal;
	char		*prompt;
	size_t		prompt_len;
}				t_suggestion_ctx;

typedef struct s_readline_state
{
	char			*buffer;
	size_t			*buffer_len;
	struct termios	*original_termios;
}					t_readline_state;

/**
 * @brief Main shell structure that encapsulates all program data
 * 
 * This structure simplifies function signatures by containing all the
 * essential data needed throughout the shell execution.
 */
typedef struct s_mshell
{
	char		*raw_command;		/* Original command before expansion */
	char		*expanded_command;	/* Command after variable expansion */
	t_envp		*envp;				/* Environment variables */
	t_ent		*tree;				/* Abstract syntax tree */
	t_config	*config;			/* Shell configuration */
	t_suggestion_ctx	*suggestions;	/* Suggestion context */
	int			exit_code;			/* Last command exit code */
	int			should_exit;		/* Flag to indicate shell should exit */
}				t_mshell;

t_envp	*envp_new_node(char *key, char *value);
t_envp	*envp_append_last(t_envp *head, t_envp *node);
t_envp	*envp_del_key(t_envp *head, char *key);
t_envp	*envp_edit_key(t_envp *head, char *key);
void	envp_clear(t_envp *head);
char	*get_env_value(t_envp *envp, const char *key);

t_ent	*ent_new_node(t_node_type type, char **argv);
int		ent_new_sibling(t_ent *node, t_ent *sibling);
int		ent_new_child(t_ent *parent, t_ent *child);
void	ent_free_node(t_ent *node);
void	ent_free(t_ent *root);

#endif