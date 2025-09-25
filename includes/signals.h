/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:19:10 by jmarcell          #+#    #+#             */
/*   Updated: 2025/09/25 05:38:07 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>

// Variables globales para tracking de señales
extern volatile sig_atomic_t	g_signal_received;

// Configuración y helpers de señales
void	set_prompt_signal(void);
void	set_child_signal(void);
void	block_parent_signals(struct sigaction *old_int, struct sigaction *old_quit);
void	restore_parent_signals(struct sigaction *old_int, struct sigaction *old_quit);

// Utilidades
int		check_signal_exit_code(void);

#endif
