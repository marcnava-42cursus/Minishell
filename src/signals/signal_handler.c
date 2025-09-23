/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:19:10 by jmarcell          #+#    #+#             */
/*   Updated: 2025/09/21 00:46:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Variable global para almacenar el exit_code cuando se recibe una señal
volatile sig_atomic_t	g_signal_received = 0;
// Variable global para rastrear si hay un proceso hijo ejecutándose
volatile sig_atomic_t	g_child_executing = 0;

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
	if (!g_child_executing)
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
void	handle_sigquit(int sig)
{
	(void)sig;
}

/**
 * @brief Configure signal handlers for the main shell process
 * 
 * Sets up custom handlers for SIGINT and SIGQUIT signals to manage
 * interactive shell behavior properly
 */
void	setup_parent_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Reset signals to default behavior
 * 
 * Restores default signal handling before exec in child processes.
 * This ensures child processes handle signals as expected by standard programs.
 */
void	reset_signals_to_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
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
