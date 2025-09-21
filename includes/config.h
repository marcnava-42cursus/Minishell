/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 17:56:23 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/20 20:49:05 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <stdlib.h>
# include <unistd.h>
# include <pwd.h>
# include <time.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "libft.h"
# include "structs.h"

# define CONFIG_FILE "minishell.config"
# define MAX_LINE_LEN 1024
# define PROMPT_BUF 64

// General configuration
int		load_config(t_config *config, char **envp);
void	free_config(t_config *config);
int		create_default_config(void);

// Build prompt
char	*build_prompt(char *raw, int exit_code);
char	*replace_keyword(char *key, int exit_code);

// Keywords
char	*get_hostname(void);
char	*get_whoami(void);
char	*get_pwd(void);
char	*get_gitbranch(void);
char	*get_jobs(void);
char	*get_battery(void);

// Colors
char	*get_color_code(const char *color);
char	*reset_color(void);

// Config utilities
char	*safe_strjoin_free(char *dst, const char *src);
int		safe_append_str(char **dst, const char *src);
int		write_config_lines(int fd, const char **lines);

// Config parsing
char	*read_single_line(int fd);
char	*extract_config_value(char *line);
int		process_config_line(char *line, char **prompt_line,
			char **suggestions_line);

// Prompt utilities
int		parse_keyword_block(const char *raw, int *i, char *kw, char *col);
int		append_single_char(char **res, const char *raw, int *i);
int		build_keyword_output(char **res, const char *kw, int exit_code,
			const char *col);

#endif
