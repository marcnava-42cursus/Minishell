/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:47 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/28 04:14:17 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*parse_var(const char **s)
{
	const char	*start;
	size_t		len;

	start = *s + 1;
	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	*s = start + len;
	return (ft_substr(start, 0, len));
}

/**
 * @brief Check if a string contains only spaces/tabs
 */
static int	has_only_whitespace(const char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Check if content contains whitespace characters
 */
static int	contains_whitespace(const char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == ' ' || str[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Process quoted content with intelligent concatenation
 */

static char	*process_quoted_content(const char *content, size_t len,
	char quote_type)
{
	char	*result;

	/* Always preserve quotes to keep operators literal until parsing stage */
	result = ft_calloc(len + 3, sizeof(char));
	if (!result)
		return (NULL);
	result[0] = quote_type;
	if (content && len > 0)
		ft_strlcpy(result + 1, content, len + 1);
	result[len + 1] = quote_type;
	return (result);
}

static char	*expand_variables_ctx(const char *in, t_envp *envp, int exit_code, int in_dquote)
{
	char		*out;
	char		*var;
	char		*val;
	char		*tmp;
	char		*num;
	const char	*start;
	char		*quote_content;
	size_t		content_len;
	char		quote;

	out = ft_strdup("");
	while (*in)
	{
		/* Handle backslashes before any expansion logic */
		if (*in == '\\')
		{
			if (in_dquote)
			{
				/* In double quotes, allow \", \\ and \$ escapes; otherwise keep backslash */
				if (*(in + 1) == '$' || *(in + 1) == '"' || *(in + 1) == '\\')
				{
					char one[2] = { *(in + 1), 0 };
					tmp = ft_strjoin(out, one);
					ft_free((void **)&out);
					out = tmp;
					in += 2;
					continue ;
				}
				else if (*(in + 1) == '\n')
				{
					in += 2;
					continue ;
				}
				/* keep backslash literal */
				char bs[2] = { '\\', 0 };
				tmp = ft_strjoin(out, bs);
				ft_free((void **)&out);
				out = tmp;
				in++;
				continue ;
			}
			/* Outside quotes: handle sequences and self-escape */
			{
				size_t k = 0;
				while (in[k] == '\\')
					k++;
				char next = in[k];
				if (next == '$')
				{
					size_t pairs = k / 2;
					while (pairs-- > 0)
					{
						char bs[2] = { '\\', 0 };
						tmp = ft_strjoin(out, bs);
						ft_free((void **)&out);
						out = tmp;
					}
					if (k % 2 == 1)
					{
						/* odd count: escape the $, output literal '$' */
						char dollar[2] = { '$', 0 };
						tmp = ft_strjoin(out, dollar);
						ft_free((void **)&out);
						out = tmp;
						in += k + 1;
						continue ;
					}
					in += k;
					continue ;
				}
				else if (next == '\n')
				{
					/* Swallow one backslash for line continuation, keep the rest */
					if (k > 1)
					{
						size_t rem = k - 1;
						while (rem-- > 0)
						{
							char bs[2] = { '\\', 0 };
							tmp = ft_strjoin(out, bs);
							ft_free((void **)&out);
							out = tmp;
						}
					}
					in += k + 1;
					continue ;
				}
				else if (next == '\\')
				{
					/* Self-escape: \\ -> \ ; consume two at a time */
					size_t pairs = k / 2;
					while (pairs-- > 0)
					{
						char bs[2] = { '\\', 0 };
						tmp = ft_strjoin(out, bs);
						ft_free((void **)&out);
						out = tmp;
					}
					if (k % 2 == 1)
					{
						/* one leftover backslash */
						char bs[2] = { '\\', 0 };
						tmp = ft_strjoin(out, bs);
						ft_free((void **)&out);
						out = tmp;
					}
					in += k;
					continue ;
				}
				else
				{
					/* Not a special sequence: keep one backslash and continue */
					char bs[2] = { '\\', 0 };
					tmp = ft_strjoin(out, bs);
					ft_free((void **)&out);
					out = tmp;
					in++;
					continue ;
				}
			}
		}
		if (ft_strncmp(in, "$?", 2) == 0)
		{
			num = ft_itoa(exit_code);
			tmp = ft_strjoin(out, num);
			ft_free((void **)&out);
			ft_free((void **)&num);
			out = tmp;
			in += 2;
		}
else if (*in == '$' && *(in + 1) == '"')
		{
			/* Treat $"..." like "..." (ignore $, expand inside) */
			quote = *(in + 1);
			in += 2;
			start = in;
			while (*in)
			{
				if (*in == '\\' && *(in + 1) != '\0')
				{
					in += 2;
					continue ;
				}
				if (*in == quote)
					break ;
				in++;
			}
			content_len = in - start;
			char *inner = ft_substr(start, 0, content_len);
			char *expanded_inner = expand_variables_ctx(inner, envp, exit_code, 1);
			ft_free((void **)&inner);
			quote_content = process_quoted_content(expanded_inner,
						expanded_inner ? ft_strlen(expanded_inner) : 0, '"');
			ft_free((void **)&expanded_inner);
			if (quote_content)
			{
				tmp = ft_strjoin(out, quote_content);
				ft_free((void **)&out);
				ft_free((void **)&quote_content);
				out = tmp;
			}
			if (*in == '"')
				in++;
		}
		else if (*in == '$' && (ft_isalpha(*(in + 1)) || *(in + 1) == '_'))
		{
			var = parse_var(&in);
			if (var && *var)
			{
				val = get_env_value(envp, var);
				if (!val)
					val = "";
				tmp = ft_strjoin(out, val);
				ft_free((void **)&out);
				out = tmp;
			}
			else
			{
				tmp = ft_strjoin(out, "$");
				ft_free((void **)&out);
				out = tmp;
				in++;
			}
			if (var)
				ft_free((void **)&var);
		}
		else if (*in == '$')
		{
			/* Treat isolated '$' as literal */
			tmp = ft_strjoin(out, "$");
			ft_free((void **)&out);
			out = tmp;
			in++;
		}
		else if (*in == '\'' && !in_dquote)
		{
			/* Single-quoted literal segment (no expansion) */
			quote = *in++;
			start = in;
			while (*in && *in != quote)
				in++;
			content_len = in - start;
			quote_content = process_quoted_content(start, content_len, quote);
			if (quote_content)
			{
				tmp = ft_strjoin(out, quote_content);
				ft_free((void **)&out);
				ft_free((void **)&quote_content);
				out = tmp;
			}
			if (*in == quote)
				in++;
		}
		else if (*in == '"')
		{
			/* Double-quoted segment: expand inside, but honor escapes */
			quote = *in++;
			start = in;
			while (*in)
			{
				if (*in == '\\' && *(in + 1) != '\0')
				{
					/* Skip escaped character so we don't prematurely close */
					in += 2;
					continue ;
				}
				if (*in == quote)
					break ;
				in++;
			}
			content_len = in - start;
			char *inner = ft_substr(start, 0, content_len);
			char *expanded_inner = expand_variables_ctx(inner, envp, exit_code, 1);
			ft_free((void **)&inner);
			quote_content = process_quoted_content(expanded_inner,
						expanded_inner ? ft_strlen(expanded_inner) : 0, '"');
			ft_free((void **)&expanded_inner);
			if (quote_content)
			{
				tmp = ft_strjoin(out, quote_content);
				ft_free((void **)&out);
				ft_free((void **)&quote_content);
				out = tmp;
			}
			if (*in == '"')
				in++;
		}
		else
		{
			/* Plain segment: copy one character */
			char one[2] = { *in, 0 };
			tmp = ft_strjoin(out, one);
			ft_free((void **)&out);
			out = tmp;
			in++;
		}
	}
	return (out);
}

char	*expand_variables(const char *in, t_envp *envp, int exit_code)
{
	return expand_variables_ctx(in, envp, exit_code, 0);
}
