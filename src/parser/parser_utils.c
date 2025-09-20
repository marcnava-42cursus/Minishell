/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:24 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 05:24:51 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "wildcards.h"
#include "utils.h"

static void	skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

static char	*get_next_token(const char **s)
{
	char		*buf;
	size_t		cap;
	size_t		out;
	const char	*p;
	int			in_squote;
	int			in_dquote;

	skip_whitespace(s);
	if (**s == '\0')
		return (NULL);
	/* Single-char and double-char operators at token start (not escaped) */
	if (**s == '(' || **s == ')')
		return (ft_substr((*s)++, 0, 1));
	if (**s == '&' && *(*s + 1) == '&')
		return ((*s) += 2, ft_strdup("&&"));
	if (**s == '|' && *(*s + 1) == '|')
		return ((*s) += 2, ft_strdup("||"));
	if (**s == '<' && *(*s + 1) == '<')
		return ((*s) += 2, ft_strdup("<<"));
	if (**s == '>' && *(*s + 1) == '>')
		return ((*s) += 2, ft_strdup(">>"));
	if (**s == '<')
		return ((*s)++, ft_strdup("<"));
	if (**s == '>')
		return ((*s)++, ft_strdup(">"));
	if (**s == '|')
		return ((*s)++, ft_strdup("|"));

	/* Build a word token honoring quotes and backslash escaping */
	p = *s;
	cap = ft_strlen(p) + 1;
	buf = ft_calloc(cap, sizeof(char));
	if (!buf)
		return (NULL);
	out = 0;
	in_squote = 0;
	in_dquote = 0;
	while (*p)
	{
		/* Token boundary (when not inside quotes) */
		if (!in_squote && !in_dquote && ( *p == ' ' || *p == '\t'
			|| *p == '(' || *p == ')' || *p == '&' || *p == '|' || *p == '<' || *p == '>'))
			break ;
		if (*p == '\\')
		{
			if (in_squote)
			{
				/* Backslash is literal inside single quotes */
				buf[out++] = *p++;
			}
			else if (in_dquote)
			{
				/* In double quotes: do not drop the backslash here; let expansion handle it */
				if (*(p + 1) == '\n')
				{
					p += 2; /* line continuation */
				}
				else
				{
					buf[out++] = *p++;
					if (*p)
						buf[out++] = *p++;
				}
			}
			else
			{
				/* Outside quotes: do not drop the backslash; include next char if any */
				if (*(p + 1) == '\n')
				{
					p += 2; /* swallow line continuation */
				}
				else
				{
					buf[out++] = *p++;
					if (*p)
						buf[out++] = *p++;
				}
			}
		}
		else if (*p == '\'' && !in_dquote)
		{
			in_squote = !in_squote;
			buf[out++] = *p++;
		}
		else if (*p == '"' && !in_squote)
		{
			in_dquote = !in_dquote;
			buf[out++] = *p++;
		}
		else
		{
			buf[out++] = *p++;
		}
	}
	buf[out] = '\0';
	*s = p;
	return (buf);
}

t_ent	*parse_cmd(const char **s, t_mshell *mshell)
{
	char	*tok;
	char	**argv;
	int		argc;
	int		fd_in;
	int		fd_out;
	char	*filename;
	int		fd;
	int		mcount;
	char	**matches;
	int		i;
	t_ent	*node;

	argv = ft_calloc(1, sizeof(char *));
	if (!argv)
		return (NULL);
	argc = 0;
	fd_in = -1;
	fd_out = -1;
	while (1)
	{
		skip_whitespace(s);
		if (!**s || **s == '|' || **s == '&' || **s == ')')
			break ;
		tok = get_next_token(s);
		if (!tok)
			break ;
		if (ft_strcmp(tok, "<<") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
print_err2("minishell: syntax error near unexpected token `newline'\n", NULL, NULL);
				mshell->exit_code = 2;
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = handle_heredoc(filename, mshell);
			ft_free((void **)&filename);
			if (fd == -1)
			{
				/* Mark failure but continue building the command list */
				mshell->exit_code = 1;
				fd_in = -2;
			}
			else
			{
				if (fd_in != -1 && fd_in != -2)
					close(fd_in);
				fd_in = fd;
			}
			continue ;
		}
		if (ft_strcmp(tok, "<") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
print_err2("minishell: syntax error near unexpected token `newline'\n", NULL, NULL);
				mshell->exit_code = 2;
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = open(filename, O_RDONLY);
			if (fd == -1)
			{
print_err2("minishell: ", filename, ": ");
				print_err2(strerror(errno), "\n", NULL);
				mshell->exit_code = 1;
				fd_in = -2;
			}
			else
			{
				if (fd_in != -1 && fd_in != -2)
					close(fd_in);
				fd_in = fd;
			}
			ft_free((void **)&filename);
			continue ;
		}
		if (ft_strcmp(tok, ">") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
print_err2("minishell: syntax error near unexpected token `newline'\n", NULL, NULL);
				mshell->exit_code = 2;
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
print_err2("minishell: ", filename, ": ");
				print_err2(strerror(errno), "\n", NULL);
				mshell->exit_code = 1;
				fd_out = -2;
			}
			else
			{
				if (fd_out != -1 && fd_out != -2)
					close(fd_out);
				fd_out = fd;
			}
			ft_free((void **)&filename);
			continue ;
		}
		if (ft_strcmp(tok, ">>") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
print_err2("minishell: syntax error near unexpected token `newline'\n", NULL, NULL);
				mshell->exit_code = 2;
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
print_err2("minishell: ", filename, ": ");
				print_err2(strerror(errno), "\n", NULL);
				mshell->exit_code = 1;
				fd_out = -2;
			}
			else
			{
				if (fd_out != -1 && fd_out != -2)
					close(fd_out);
				fd_out = fd;
			}
			ft_free((void **)&filename);
			continue ;
		}
		/* Normal argument or pattern */
		if (wc_token_should_expand(tok))
		{
			mcount = 0;
			matches = wc_expand(tok, &mcount);
			if (matches && mcount > 0)
			{
				i = 0;
				while (i < mcount)
				{
					argv = ft_realloc_matrix(argv, argc, matches[i]);
					argc++;
					i++;
				}
				ft_free((void **)&tok);
				ft_free((void **)&matches);
			}
			else
			{
				argv = ft_realloc_matrix(argv, argc, tok);
				argc++;
			}
		}
		else
		{
			argv = ft_realloc_matrix(argv, argc, tok);
			argc++;
		}
	}
	argv = ft_realloc_matrix(argv, argc, NULL);
	if (!argv)
		return (NULL);
	argv[argc] = NULL;
	node = ent_new_node(NODE_COMMAND, argv);
	if (node)
	{
		node->fd_in = fd_in;
		node->fd_out = fd_out;
	}
	return (node);
}

t_ent	*parse_subshell(const char **s, t_mshell *mshell)
{
	const char	*start;
	const char	*p;
	int			depth;
	char		*content;
	t_ent		*child;
	t_ent		*node;
	char		**argv;

	(*s)++;
	start = *s;
	depth = 1;
	p = start;
	while (*p && depth)
	{
		if (*p == '(')
			depth++;
		else if (*p == ')')
			depth--;
		p++;
	}
	content = ft_substr(start, 0, (size_t)(p - start - 1));
	if (!content)
		return (NULL);
	child = parse_list(s, mshell);
	if (**s == ')')
		(*s)++;
	argv = ft_calloc(2, sizeof(char *));
	if (!argv)
		return (ft_free((void **)&content), NULL);
	argv[0] = content;
	node = ent_new_node(NODE_SUBSHELL, argv);
	if (node)
		node->child = child;
	return (node);
}

/**
 * @brief Check if heredoc delimiter in original command has quotes
 * 
 * @param cmd The original command string (before expansion)
 * @param delimiter_pos Position hint (can be NULL)
 * @return 1 if delimiter is quoted, 0 if not, -1 on error
 */
int	check_heredoc_quotes(const char *cmd, const char *delimiter_pos)
{
	const char	*search_pos;

	if (!delimiter_pos)
		delimiter_pos = NULL;
	if (!cmd)
		return (-1);
	search_pos = ft_strnstr(cmd, "<<", ft_strlen(cmd));
	if (!search_pos)
		return (-1);
	search_pos += 2;
	while (*search_pos == ' ' || *search_pos == '\t')
		search_pos++;
	if (*search_pos == '\'' || *search_pos == '\"')
		return (1);
	return (0);
}

/**
 * @brief Get delimiter from original command without expansion processing
 * 
 * @param cmd The original command string (before expansion)
 * @return Original delimiter, or NULL on error
 */
static char	*get_original_delimiter_from_cmd(const char *cmd)
{
	const char	*search_pos;
	const char	*delim_start;
	const char	*delim_end;
	char		quote;
	size_t		len;

	if (!cmd)
		return (NULL);
	search_pos = ft_strnstr(cmd, "<<", ft_strlen(cmd));
	if (!search_pos)
		return (NULL);
	search_pos += 2;
	while (*search_pos == ' ' || *search_pos == '\t')
		search_pos++;
	delim_start = search_pos;
	if (*delim_start == '\'' || *delim_start == '\"')
	{
		quote = *delim_start;
		delim_end = delim_start + 1;
		while (*delim_end && *delim_end != quote)
			delim_end++;
		if (*delim_end == quote)
			delim_end++;
	}
	else
	{
		delim_end = delim_start;
		while (*delim_end && *delim_end != ' ' && *delim_end != '\t'
			&& *delim_end != '\n' && *delim_end != '|' && *delim_end != '&')
			delim_end++;
	}
	len = delim_end - delim_start;
	return (ft_substr(delim_start, 0, len));
}

/**
 * @brief Check if delimiter is quoted and return clean delimiter
 * 
 * @param delimiter Original delimiter (may have quotes)
 * @param is_quoted Output parameter - set to 1 if quoted, 0 if not
 * @return Clean delimiter without quotes, or NULL on error
 */
static char	*process_heredoc_delimiter(const char *delimiter, int *is_quoted)
{
	size_t	len;
	char	quote;

	if (!delimiter || !is_quoted)
		return (NULL);
	*is_quoted = 0;
	len = ft_strlen(delimiter);
	if (len >= 2 && (delimiter[0] == '\'' || delimiter[0] == '\"'))
	{
		quote = delimiter[0];
		if (delimiter[len - 1] == quote)
		{
			*is_quoted = 1;
			return (ft_substr(delimiter, 1, len - 2));
		}
	}
	return (ft_strdup(delimiter));
}

/**
 * @brief Handle heredoc by creating a temporary file and reading input
 *        until delimiter
 * 
 * @param delimiter The heredoc delimiter string (already processed by
 *                  expand_variables) - UNUSED
 * @param mshell The shell structure containing env and other data
 * @return File descriptor of the temporary file, or -1 on error
 */
int	handle_heredoc(const char *delimiter, t_mshell *mshell)
{
	static int	heredoc_count = 0;
	char		*tmp_filename;
	char		*line;
	char		*expanded_line;
	char		*clean_delimiter;
	char		*original_delimiter;
	char		*count_str;
	int			tmp_fd;
	int			read_fd;
	int			is_quoted;

	(void)delimiter;
	if (!mshell || !mshell->raw_command)
		return (-1);
	original_delimiter = get_original_delimiter_from_cmd(mshell->raw_command);
	if (!original_delimiter)
		return (-1);
	is_quoted = check_heredoc_quotes(mshell->raw_command, NULL);
	if (is_quoted < 0)
		is_quoted = 0;
	else if (is_quoted > 0)
		is_quoted = 1;
	clean_delimiter = process_heredoc_delimiter(original_delimiter, &is_quoted);
	ft_free((void **)&original_delimiter);
	if (!clean_delimiter)
		return (-1);
	count_str = ft_itoa(heredoc_count++);
	if (!count_str)
		return (ft_free((void **)&clean_delimiter), -1);
	tmp_filename = ft_strjoin("/tmp/heredoc_", count_str);
	ft_free((void **)&count_str);
	if (!tmp_filename)
		return (ft_free((void **)&clean_delimiter), -1);
	tmp_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
	{
		perror("heredoc: open");
		ft_free((void **)&clean_delimiter);
		ft_free((void **)&tmp_filename);
		return (-1);
	}
	printf("> ");
	while ((line = readline("")) != NULL)
	{
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			ft_free((void **)&line);
			break ;
		}
		if (is_quoted)
			write(tmp_fd, line, ft_strlen(line));
		else
		{
			expanded_line = expand_variables(line, mshell->envp,
					mshell->exit_code);
			if (expanded_line)
			{
				write(tmp_fd, expanded_line, ft_strlen(expanded_line));
				ft_free((void **)&expanded_line);
			}
			else
				write(tmp_fd, line, ft_strlen(line));
		}
		write(tmp_fd, "\n", 1);
		ft_free((void **)&line);
		printf("> ");
	}
	close(tmp_fd);
	read_fd = open(tmp_filename, O_RDONLY);
	if (read_fd == -1)
	{
		perror("heredoc: reopen");
		unlink(tmp_filename);
		ft_free((void **)&tmp_filename);
		ft_free((void **)&clean_delimiter);
		return (-1);
	}
	unlink(tmp_filename);
	ft_free((void **)&tmp_filename);
	ft_free((void **)&clean_delimiter);
	return (read_fd);
}
