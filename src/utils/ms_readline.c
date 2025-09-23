/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 21:36:48 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:43:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*read_line_dispatch(t_mshell *ms)
{
	if (ms->config->use_suggestions && ms->suggestions)
		return (suggestion_readline(ms->suggestions));
	if (isatty(STDIN_FILENO))
		return (read_line_tty(ms));
	return (read_line_nontty());
}

char	*read_line_tty(const t_mshell *ms)
{
	return (readline(ms->config->prompt));
}

char	*read_line_nontty(void)
{
	char	buffer[4096];
	char	ch;
	char	*line;
	int		i;
	int		r;

	i = 0;
	r = read(STDIN_FILENO, &ch, 1);
	while (i < 4095 && r > 0)
	{
		if (ch == '\n')
			break ;
		buffer[i++] = ch;
		r = read(STDIN_FILENO, &ch, 1);
	}
	if (r <= 0 && i == 0)
		return (NULL);
	buffer[i] = '\0';
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	ft_strlcpy(line, buffer, (size_t)i + 1);
	return (line);
}

void	process_line(t_mshell *ms, char *line)
{
	add_history(line);
	if (parse_and_maybe_exec(ms, line) != 0 && isatty(STDIN_FILENO))
		printf("Error parsing command\n");
	refresh_prompt_and_sugg(ms);
}

void	refresh_prompt_and_sugg(t_mshell *ms)
{
	if (ms->config->prompt)
		ft_free((void **)&ms->config->prompt);
	ms->config->prompt = build_prompt(ms->config->prompt_raw,
			ms->exit_code);
	if (ms->config->use_suggestions && ms->suggestions)
		suggestion_update_prompt(ms->suggestions, ms->config->prompt);
}
