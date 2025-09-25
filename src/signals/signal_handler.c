/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:19:10 by jmarcell          #+#    #+#             */
/*   Updated: 2025/09/25 05:32:23 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

// Variable global para almacenar el exit_code cuando se recibe una señal
volatile sig_atomic_t	g_signal_received = 0;

static void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
}

/**
 * @brief Handle SIGINT signal (Ctrl+C)
 * 
 * In interactive mode: displays new line and shows new prompt
 * In non-interactive mode: terminates the shell
 * 
 * @param sig The signal number received
 */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	if (isatty(STDIN_FILENO))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/**
 * @brief Handle SIGQUIT signal (Ctrl+\)
 * 
 * In interactive mode: ignores the signal
 * Child processes should receive it normally
 * 
 * @param sig The signal number received
 */
/* No usamos handler de SIGQUIT en el padre; se ignora vía sigaction */

/**
 * @brief Configura señales para el prompt interactivo del padre
 */
void	set_prompt_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Reset signals to default behavior
 * 
 * Restores default signal handling before exec in child processes.
 * This ensures child processes handle signals as expected by standard programs.
 */
/**
 * @brief Señales por defecto en el proceso hijo antes de exec
 */
void	set_child_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
}

void	set_heredoc_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = heredoc_handler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Bloquea señales en el padre (ignorando SIGINT/SIGQUIT) guardando handlers previos
 */
void	block_parent_signals(t_mshell *ms)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &ms->old_sigint);
	sigaction(SIGQUIT, &sa, &ms->old_sigquit);
}

/**
 * @brief Restaura handlers previos en el padre tras esperar al hijo
 */
void	restore_parent_signals(t_mshell *ms)
{
	sigaction(SIGINT, &ms->old_sigint, NULL);
	sigaction(SIGQUIT, &ms->old_sigquit, NULL);
}

/**
 * @brief Check if a signal was received and return appropriate exit code
 * 
 * Checks the global signal flag and returns the corresponding exit code.
 * Resets the signal flag after checking.
 * 
 * @return Exit code based on received signal (130 for SIGINT, 0 for none)
 */
int	check_signal_exit_code(void)
{
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		return (130);
	}
	return (0);
}
