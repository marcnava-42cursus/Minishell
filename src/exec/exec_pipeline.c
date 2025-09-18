/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 23:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	close_all_pipes_local(int **pipes, int cmd_count);

void	exec_pipeline_child(t_pipe_ctx *ctx, int i)
{
	setup_child_signals();
	setup_input_redirection(ctx->pipes, ctx->commands[i], i);
	setup_output_redirection(ctx->pipes, ctx->commands[i], i, ctx->cmd_count);
	close_all_pipes_local(ctx->pipes, ctx->cmd_count);
	if (is_builtin(ctx->commands[i]->argv[0]))
		exit(exec_builtin(ctx->commands[i], ctx->mshell));
	handle_child_process(ctx->commands[i], ctx->mshell,
		envp_to_array(ctx->mshell->envp));
}

static void	close_all_pipes_local(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

static int	init_ctx(t_pipe_ctx *ctx, t_ent *node, t_mshell *mshell)
{
	ctx->cmd_count = count_pipeline_commands(node);
	if (ctx->cmd_count <= 0)
		return (1);
	ctx->mshell = mshell;
	ctx->commands = malloc(sizeof(t_ent *) * ctx->cmd_count);
	ctx->pipes = malloc(sizeof(int *) * (ctx->cmd_count - 1));
	ctx->pids = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->commands || !ctx->pipes || !ctx->pids)
		return (1);
	collect_pipeline_commands(node, ctx->commands, ctx->cmd_count);
	if (setup_pipeline_pipes(ctx->pipes, ctx->cmd_count))
		return (1);
	return (0);
}

static int	wait_and_cleanup(t_pipe_ctx *ctx)
{
	int	status;
	int	i;

	close_all_pipes_local(ctx->pipes, ctx->cmd_count);
	set_child_executing();
	i = 0;
	while (i < ctx->cmd_count)
	{
		waitpid(ctx->pids[i], &status, 0);
		i++;
	}
	unset_child_executing();
	i = 0;
	while (i < ctx->cmd_count - 1)
	{
		ft_free((void **)&ctx->pipes[i]);
		i++;
	}
	ft_free((void **)&ctx->commands);
	ft_free((void **)&ctx->pipes);
	ft_free((void **)&ctx->pids);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_pipeline(t_ent *node, t_mshell *mshell)
{
	t_pipe_ctx	ctx;

	if (count_pipeline_commands(node) == 1)
		return (single_command_from_pipeline(node, mshell));
	if (init_ctx(&ctx, node, mshell))
		return (1);
	if (spawn_children(&ctx))
		return (1);
	return (wait_and_cleanup(&ctx));
}
