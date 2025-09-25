/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:19:10 by jmarcell          #+#    #+#             */
/*   Updated: 2025/09/20 20:48:36 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>

// Variables globales para tracking de señales
extern volatile sig_atomic_t	g_signal_received;

// Manejadores de señales
void	handle_sigint(int sig);
void	setup_parent_signals(void);
void	reset_signals_to_default(void);
void	set_child_signal(void);

// Utilidades
int		check_signal_exit_code(void);

#endif
