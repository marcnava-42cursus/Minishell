/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:57:24 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/21 02:33:03 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "wildcards.h"
#include "utils.h"

/**
 * @brief Check if heredoc delimiter in original command has quotes
 * 
 * @param cmd The original command string (before expansion)
 * @param delimiter_pos Position hint (can be NULL)
 * @return 1 if delimiter is quoted, 0 if not, -1 on error
 */
/**
 * @brief Get delimiter from original command without expansion processing
 * 
 * @param cmd The original command string (before expansion)
 * @return Original delimiter, or NULL on error
 */
/**
 * @brief Check if delimiter is quoted and return clean delimiter
 * 
 * @param delimiter Original delimiter (may have quotes)
 * @param is_quoted Output parameter - set to 1 if quoted, 0 if not
 * @return Clean delimiter without quotes, or NULL on error
 */
/**
 * @brief Handle heredoc by creating a temporary file and reading input
 *        until delimiter
 * 
 * @param delimiter The heredoc delimiter string (already processed by
 *                  expand_variables) - UNUSED
 * @param mshell The shell structure containing env and other data
 * @return File descriptor of the temporary file, or -1 on error
 */
