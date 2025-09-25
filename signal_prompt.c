/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruiz-d <anruiz-d@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:34:59 by anruiz-d          #+#    #+#             */
/*   Updated: 2025/09/01 17:28:29 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Only use readline functions when in TTY mode
static void	handler_signal(int sign)
{
	if (sign == SIGINT)
	{
		(void)sign;
		write(1, "\n", 1);
		if (isatty(STDIN_FILENO))
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (sign == SIGQUIT)
		(void)sign;
}

void	set_prompt_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler_signal;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("Error: Signal Ctrl + C");
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("Error: Signal Ctrl + \\");
}

void	set_child_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("Error: Signal Crtl + C");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("Error: Signal Crtl + D");
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
		perror("Error: Signal SIGPIPE");
}

void	block_parent_signals(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, old_int) < 0)
		perror("error sigaction C in block");
	if (sigaction(SIGQUIT, &sa, old_quit) < 0)
		perror("error sigaction / in block");
}

void	restore_parent_signals(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	if (sigaction(SIGINT, old_int, NULL) < 0)
		perror("error sigaction C in block");
	if (sigaction(SIGQUIT, old_quit, NULL) < 0)
		perror("error sigaction / in block");
}
