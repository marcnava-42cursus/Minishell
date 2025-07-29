/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:43:15 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/30 20:44:59 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// Helper function to create a command node from a string
static t_ent	*create_command_node(char *cmd_str)
{
	t_ent	*cmd;
	char	**argv;

	if (!cmd_str || !*cmd_str)
		return (NULL);
	// Remove leading and trailing whitespace
	while (*cmd_str == ' ' || *cmd_str == '\t')
		cmd_str++;
	if (!*cmd_str)
		return (NULL);
	argv = ft_split(cmd_str, ' ');
	if (!argv || !argv[0])
	{
		if (argv)
			ft_free_matrix((void **)argv);
		return (NULL);
	}
	cmd = ent_new_node(NODE_COMMAND, argv);
	if (!cmd)
		ft_free_matrix((void **)argv);
	return (cmd);
}

// Parse pipes and create a pipeline tree
t_ent	*parse_pipeline(char *input)
{
	char	**pipe_parts;
	t_ent	*root;
	t_ent	*pipe_node;
	t_ent	*right_cmd;
	int		i;

	if (!input)
		return (NULL);
	// Split by pipes
	pipe_parts = ft_split(input, '|');
	if (!pipe_parts)
		return (NULL);
	// If no pipes, just parse as simple command
	if (!pipe_parts[1])
	{
		root = create_command_node(pipe_parts[0]);
		ft_free_matrix((void **)pipe_parts);
		return (root);
	}
	// Create the first command
	root = create_command_node(pipe_parts[0]);
	if (!root)
	{
		ft_free_matrix((void **)pipe_parts);
		return (NULL);
	}
	// Chain the rest with pipe nodes
	i = 1;
	while (pipe_parts[i])
	{
		// Create pipe node
		pipe_node = ent_new_node(NODE_PIPE, NULL);
		if (!pipe_node)
		{
			ent_free(root);
			ft_free_matrix((void **)pipe_parts);
			return (NULL);
		}
		// Create right command
		right_cmd = create_command_node(pipe_parts[i]);
		if (!right_cmd)
		{
			ent_free_node(pipe_node);
			ent_free(root);
			ft_free_matrix((void **)pipe_parts);
			return (NULL);
		}
		// Set pipe node children: left is current root, right is new command
		ent_new_child(pipe_node, root);
		ent_new_child(pipe_node, right_cmd);
		// Update root to be the pipe node
		root = pipe_node;
		i++;
	}
	ft_free_matrix((void **)pipe_parts);
	return (root);
}

t_ent	*parse_simple_command(char *input)
{
	return (parse_pipeline(input));
}
