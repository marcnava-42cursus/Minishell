/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarcell <jmarcell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:24 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/25 16:41:05 by jmarcell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

static char *get_next_token(const char **s)
{
	const char *start;
	size_t len;

	skip_whitespace(s);
	if (**s == '\0')
		return (NULL);
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

	start = *s;
	len = 0;
	while ((*s)[len] && !ft_strchr(" \t()&|<>", (*s)[len]))
	{
		/* Handle quoted sections within a token */
		if ((*s)[len] == '"' || (*s)[len] == '\'')
		{
			char quote = (*s)[len];
			len++; /* Include opening quote */
			/* Skip to closing quote */
			while ((*s)[len] && (*s)[len] != quote)
				len++;
			if ((*s)[len] == quote)
				len++; /* Include closing quote */
		}
		else
			len++;
	}
	*s += len;
	return (ft_substr(start, 0, len));
}

t_ent *parse_cmd(const char **s)
{
	char *tok;
	char **argv;
	int argc;
	t_ent *node;
	char *filename;
	int fd;

	skip_whitespace(s);
	tok = get_next_token(s);
	if (!tok)
		return (NULL);
	
	// Handle case where command starts with redirection
	if (ft_strcmp(tok, "<<") == 0 || ft_strcmp(tok, "<") == 0 || 
		ft_strcmp(tok, ">") == 0 || ft_strcmp(tok, ">>") == 0)
	{
		// This shouldn't happen in a well-formed command, but handle gracefully
		ft_free((void **)&tok);
		return (NULL);
	}
	
	argv = ft_calloc(2, sizeof(char *));
	if (!argv)
		return (ft_free((void **)&tok), NULL);
	argv[0] = tok;
	argc = 1;
	
	// Parse arguments and redirections
	while (1)
	{
		skip_whitespace(s);
		if (!**s || **s == '|' || **s == '&' || **s == ')')
			break;
		
		tok = get_next_token(s);
		if (!tok)
			break;
		
		// Handle redirections
		if (ft_strcmp(tok, "<<") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = handle_heredoc(filename);
			ft_free((void **)&filename);
			if (fd == -1)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			node = ent_new_node(NODE_COMMAND, argv);
			if (node)
				node->fd_in = fd;
			return (node);
		}
		else if (ft_strcmp(tok, "<") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = open(filename, O_RDONLY);
			ft_free((void **)&filename);
			if (fd == -1)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			node = ent_new_node(NODE_COMMAND, argv);
			if (node)
				node->fd_in = fd;
			return (node);
		}
		else if (ft_strcmp(tok, ">") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			ft_free((void **)&filename);
			if (fd == -1)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			node = ent_new_node(NODE_COMMAND, argv);
			if (node)
				node->fd_out = fd;
			return (node);
		}
		else if (ft_strcmp(tok, ">>") == 0)
		{
			ft_free((void **)&tok);
			filename = get_next_token(s);
			if (!filename)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			ft_free((void **)&filename);
			if (fd == -1)
			{
				ft_free_matrix((void **)argv);
				return (NULL);
			}
			node = ent_new_node(NODE_COMMAND, argv);
			if (node)
				node->fd_out = fd;
			return (node);
		}
		else
		{
			// Regular argument
			argv = ft_realloc_matrix(argv, argc, tok);
			argc++;
		}
	}
	
	argv[argc] = NULL;
	return (ent_new_node(NODE_COMMAND, argv));
}

t_ent *parse_subshell(const char **s)
{
	const char *start;
	const char *p;
	int depth;
	char *content;
	t_ent *child;
	t_ent *node;
	char **argv;

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
	child = parse_list(s);
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
 * @brief Handle heredoc by creating a temporary file and reading input until delimiter
 * 
 * @param delimiter The heredoc delimiter string
 * @return File descriptor of the temporary file, or -1 on error
 */
int	handle_heredoc(const char *delimiter)
{
	static int	heredoc_count = 0;
	char		*tmp_filename;
	char		*line;
	char		*count_str;
	int			tmp_fd;
	int			read_fd;

	if (!delimiter)
		return (-1);

	// Create unique temporary filename
	count_str = ft_itoa(heredoc_count++);
	if (!count_str)
		return (-1);
	
	tmp_filename = ft_strjoin("tmp/heredoc_", count_str);
	free(count_str);
	if (!tmp_filename)
		return (-1);

	// Create temporary file
	tmp_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
	{
		perror("heredoc: open");
		free(tmp_filename);
		return (-1);
	}

	// Read input until delimiter is found
	printf("> ");
	while ((line = readline("")) != NULL)
	{
		// Check if line matches delimiter
		if (ft_strcmp(line, (char *)delimiter) == 0)
		{
			free(line);
			break;
		}
		
		// Write line to temporary file with newline
		write(tmp_fd, line, ft_strlen(line));
		write(tmp_fd, "\n", 1);
		
		free(line);
		printf("> ");
	}

	// Close write fd
	close(tmp_fd);

	// Open for reading
	read_fd = open(tmp_filename, O_RDONLY);
	if (read_fd == -1)
	{
		perror("heredoc: reopen");
		unlink(tmp_filename);
		free(tmp_filename);
		return (-1);
	}

	// Schedule file for deletion (will be deleted when fd is closed)
	unlink(tmp_filename);
	free(tmp_filename);

	return (read_fd);
}
