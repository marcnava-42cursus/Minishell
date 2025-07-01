#ifndef CONFIG_H
# define CONFIG_H

# include <stdlib.h>
# include "libft.h"
# include "structs.h"

# define CONFIG_FILE "minishell.config"
# define MAX_LINE_LEN 1024

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
// char	*get_time(void);
// char	*get_datetime(void);
// char	*get_exitcode(int exitcode);
char	*get_jobs(void);
char	*get_battery(void);

// Colors
char	*get_color_code(char *color);
char	*reset_color(void);

#endif
