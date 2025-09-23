/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 21:11:39 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/23 21:12:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "utils.h"

void	wait_children(const t_pipe_ctx *ctx, int *last_status,
			int *first_sigpipe)
{
	int	i;
	int	st;

	*first_sigpipe = 0;
	i = 0;
	while (i < ctx->cmd_count)
	{
		waitpid(ctx->pids[i], &st, 0);
		if (i == 0 && WIFSIGNALED(st) && WTERMSIG(st) == SIGPIPE)
			*first_sigpipe = 1;
		*last_status = st;
		i++;
	}
}

void	free_pipe_rows(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		ft_free((void **)&pipes[i]);
		i++;
	}
}

int	should_print_broken_pipe(const t_pipe_ctx *ctx, int first_sigpipe)
{
	int	first_is_builtin;

	if (!first_sigpipe || ctx->cmd_count <= 0
		|| !ctx->commands || !ctx->commands[0]
		|| !ctx->commands[0]->argv || !ctx->commands[0]->argv[0])
		return (0);
	first_is_builtin = is_builtin(ctx->commands[0]->argv[0]);
	return (!first_is_builtin);
}

int	status_to_exitcode(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	free_ctx_arrays(t_pipe_ctx *ctx)
{
	ft_free((void **)&ctx->commands);
	ft_free((void **)&ctx->pipes);
	ft_free((void **)&ctx->pids);
}
