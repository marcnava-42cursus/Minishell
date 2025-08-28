/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:47 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/27 20:42:15 by marcnava         ###   ########.fr       */
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

static char	*process_quoted_content(const char *content, size_t len, char quote_type)
{
	char	*result;

	if (len == 0)
	{
		return (ft_strdup(""));
	}
	if (has_only_whitespace(content, len) && len <= 2)
	{
		return (ft_substr(content, 0, len));
	}
	if (contains_whitespace(content, len))
	{
		result = ft_calloc(len + 3, sizeof(char));
		if (!result)
			return (NULL);
		result[0] = quote_type;
		ft_strlcpy(result + 1, content, len + 1);
		result[len + 1] = quote_type;
		return (result);
	}
	return (ft_substr(content, 0, len));
}

char	*expand_variables(const char *in, t_envp *envp, int exit_code)
{
	char		*out;
	char		*var;
	char		*val;
	char		*tmp;
	char		*seg;
	char		*num;
	const char	*start;
	char		*quote_content;
	size_t		content_len;
	char		quote;

	out = ft_strdup("");
	while (*in)
	{
		if (ft_strncmp(in, "$?", 2) == 0)
		{
			num = ft_itoa(exit_code);
			tmp = ft_strjoin(out, num);
			ft_free((void **)&out);
			ft_free((void **)&num);
			out = tmp;
			in += 2;
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
		else if (*in == '\'' || *in == '\"')
		{
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
		else
		{
			start = in;
			while (*in && *in != '$' && *in != '\"' && *in != '\'')
				in++;
			seg = ft_substr(start, 0, in - start);
			tmp = ft_strjoin(out, seg);
			ft_free((void **)&out);
			ft_free((void **)&seg);
			out = tmp;
		}
	}
	return (out);
}
