/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setups.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:54:33 by val               #+#    #+#             */
/*   Updated: 2025/05/18 13:53:46 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

static void	signal_prompt_handler(int sig);

void	setup_signals_prompt(void)
{
	struct sigaction	s_sigaction;

	sigemptyset(&s_sigaction.sa_mask);
	s_sigaction.sa_handler = signal_prompt_handler;
	s_sigaction.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s_sigaction, NULL);
	sigaction(SIGTERM, &s_sigaction, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	signal_prompt_handler(int sig)
{
	g_signal_status = sig;
	if (sig == SIGINT)
		handle_prompt_sigint();
	else if (sig == SIGQUIT)
		handle_prompt_sigquit();
}
