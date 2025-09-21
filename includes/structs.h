/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:01:01 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:46:26 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/* Dependencias mínimas para tipos en estructuras */
# include <stdio.h>
# include <sys/types.h>
# include <termios.h>
# include "libft.h"

/* ============================= Generales ================================= */
/* Constantes generales de la aplicación */
# define SUG_BUFFER_SIZE 1024
# define CMD_INITIAL_CAPACITY 256

/* =========================== Parser helpers ============================== */
/* Contexto local para construir argv/FDs en parse_cmd */
typedef struct s_pc_ctx
{
	char	**argv;
	int		argc;
	int		fd_in;
	int		fd_out;
}t_pc_ctx;

/* Contexto interno del tokenizer */
typedef struct s_tokctx
{
	char		*buf;
	size_t		cap;
	size_t		out;
	const char	*p;
	int			in_squote;
	int			in_dquote;
}t_tokctx;

/* Contexto para heredoc */
typedef struct s_hd_ctx
{
	char	*tmp_filename;
	char	*clean_delimiter;
	char	*original_delimiter;
	char	*count_str;
	int		tmp_fd;
	int		read_fd;
	int		is_quoted;
}t_hd_ctx;

/*
** Lista enlazada simple para variables de entorno (KEY=VALUE)
*/
typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}					t_envp;

/*
** Configuración general de Minishell (prompt y flags)
*/
typedef struct s_config
{
	int		use_suggestions;
	char	*prompt_raw;
	char	*prompt;
}			t_config;

/* =============================== Parseo ================================== */
/*
** Tipo de nodo para el árbol de sintaxis (AST)
*/
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

/*
** Nodo del AST: comando, pipe, operadores lógicos y subshells
*/
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

/* ============================ Funcionalidad ============================== */
/* ---- Sugerencias -------------------------------------------------------- */
/* Lista dinámica de comandos disponibles para autocompletado/sugerencias */
typedef struct s_cmd_list
{
	char	**items;
	size_t	count;
	size_t	capacity;
}			t_cmd_list;

/* Capacidades del terminal necesarias para pintar sugerencias */
typedef struct s_terminal
{
	int				capabilities_loaded;
	char			*clear_to_eol;
	char			*cursor_move;
}					t_terminal;

/* Contexto de sugerencias: lista de comandos y estado de prompt */
typedef struct s_suggestion_ctx
{
	t_cmd_list	*commands;
	t_terminal	*terminal;
	char		*prompt;
	size_t		prompt_len;
}				t_suggestion_ctx;

/* ---- Wildcards ---------------------------------------------------------- */
/* Estado para iterar rutas base durante la expansión de comodines */
typedef struct s_walk
{
	char	**bases;
	int		basec;
	int		i;
	int		c;
}			t_walk;

/* Contexto para avanzar un segmento del patrón de ruta */
typedef struct s_adv
{
	char		**bases;
	int			basec;
	const char	*seg;
	int			is_last;
}				t_adv;

/* Contexto para leer un directorio y acumular coincidencias */
typedef struct s_dirctx
{
	char		***out;
	int			*outc;
	const char	*base;
	const char	*segment;
}				t_dirctx;

/* Estado interno del matcher de '*' (no usar "struct" en .c) */
typedef struct s_mw
{
	int			i;
	int			j;
	int			star;
	int			mark;
	const char	*s;
	const char	*p;
}				t_mw;

/* ---- Forkerman (mini-juego) -------------------------------------------- */
/* Constantes del tablero y bombas */
# define WIDTH		21
# define HEIGHT		11
# define MAX_BOMBS	99
# define BOMB_TIMER	5

/* Bomba con posición y temporizador */
typedef struct s_bomb
{
	int	x;
	int	y;
	int	timer;
}		t_bomb;

/* Estado del juego: jugador, bombas, mapa y atributos de terminal */
typedef struct s_game
{
	int				player_x;
	int				player_y;
	int				lives;
	int				bombs_available;
	int				bomb_count;
	t_bomb			bombs[MAX_BOMBS];
	char			map[HEIGHT][WIDTH + 1];
	struct termios	terminal_attrs;
}					t_game;

/* ================================ Utiles ================================= */
/* Estado de readline para conmutar modo raw del terminal */
typedef struct s_readline_state
{
	char			*buffer;
	size_t			*buffer_len;
	struct termios	*original_termios;
}					t_readline_state;

/* =============================== Core ==================================== */
/*
** Estructura principal del shell; centraliza datos globales para simplificar
** firmas de funciones y acceso al estado.
*/
typedef struct s_mshell
{
	char				*raw_command;
	char				*expanded_command;
	t_envp				*envp;
	t_ent				*tree;
	t_config			*config;
	t_suggestion_ctx	*suggestions;
	int					exit_code;
	int					should_exit;
}						t_mshell;

/* ============================== Ejecución ================================ */
/*
** Contexto para la ejecución de pipelines: comandos, pipes y PIDs
*/
typedef struct s_pipe_ctx
{
	t_ent		**commands;
	int			**pipes;
	pid_t		*pids;
	int			cmd_count;
	t_mshell	*mshell;
}				t_pipe_ctx;

/* ============================= Prototipos ================================ */
/* Funciones asociadas a listas de entorno */
t_envp	*envp_new_node(char *key, char *value);
t_envp	*envp_append_last(t_envp *head, t_envp *node);
t_envp	*envp_del_key(t_envp *head, char *key);
t_envp	*envp_edit_key(t_envp *head, char *key);
void	envp_clear(t_envp *head);
char	*get_env_value(t_envp *envp, const char *key);

/* Funciones asociadas al árbol de sintaxis */
t_ent	*ent_new_node(t_node_type type, char **argv);
int		ent_new_sibling(t_ent *node, t_ent *sibling);
int		ent_new_child(t_ent *parent, t_ent *child);
void	ent_free_node(t_ent *node);
void	ent_free(t_ent *root);

#endif
