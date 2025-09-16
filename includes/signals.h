/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarcell <jmarcell@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:19:10 by jmarcell          #+#    #+#             */
/*   Updated: 2025/09/09 20:19:10 by jmarcell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>

// Variables globales para tracking de señales
extern volatile sig_atomic_t g_signal_received;
extern volatile sig_atomic_t g_child_executing;

// Manejadores de señales
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

// Configuración de señales
void	setup_parent_signals(void);
void	setup_child_signals(void);
void	reset_signals_to_default(void);

// Gestión del estado de ejecución
void	set_child_executing(void);
void	unset_child_executing(void);

// Utilidades
int		check_signal_exit_code(void);

#endif
