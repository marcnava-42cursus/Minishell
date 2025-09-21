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
 * Manejador para SIGINT (Ctrl+C)
 * En modo interactivo: muestra nueva línea y nuevo prompt
 * En modo no-interactivo: termina el shell
 */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	// Si no hay un proceso hijo ejecutándose, estamos en el prompt
	if (!g_child_executing)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// Si hay un proceso hijo, solo interrumpimos
}

/**
 * Manejador para SIGQUIT (Ctrl+\)
 * En modo interactivo: ignora la señal
 * Los procesos hijos sí deben recibirla
 */
void	handle_sigquit(int sig)
{
	(void)sig;
	// En modo interactivo, el shell ignora SIGQUIT
	// Solo los procesos hijos deben recibirla
}

/**
 * Configura los manejadores de señales para el proceso principal (shell)
 */
void	setup_parent_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Configurar manejador para SIGINT
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART; // Reinicia syscalls interrumpidas
	sigaction(SIGINT, &sa_int, NULL);
	// Configurar manejador para SIGQUIT (ignorar)
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * Restaura el comportamiento por defecto de las señales
 * Se usa antes de hacer exec en procesos hijos
 */
void	reset_signals_to_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	/* Restaurar SIGPIPE por defecto: el escritor muere con SIGPIPE y se reporta "Broken pipe" */
	signal(SIGPIPE, SIG_DFL);
}

/**
 * Configura las señales para procesos hijos
 * Los hijos deben recibir las señales normalmente
 */
void	setup_child_signals(void)
{
	reset_signals_to_default();
}

/**
 * Marca que se va a ejecutar un proceso hijo
 */
void	set_child_executing(void)
{
	g_child_executing = 1;
}

/**
 * Marca que se termino de ejecutar un proceso hijo
 */
void	unset_child_executing(void)
{
	g_child_executing = 0;
}

/**
 * Verifica si se ha recibido una señal y actualiza el exit_code
 */
int	check_signal_exit_code(void)
{
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		return (130); // Exit code estándar para SIGINT
	}
	return (0);
}
