/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setups.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:54:33 by val               #+#    #+#             */
/*   Updated: 2025/05/19 02:42:51 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

static void	signal_prompt_handler(int sig);
static void	signal_parent_handler(int sig);
static void	signal_heredoc_handler(int sig);

void	setup_signals(t_signal_context context)
{
	struct sigaction	s_sigaction;
	void				(*handler)(int);

	if (context == SIGCONTEXT_PROMPT)
		handler = signal_prompt_handler;
	else if (context == SIGCONTEXT_FORK)
		handler = SIG_DFL;
	else if (context == SIGCONTEXT_PARENT)
		handler = signal_parent_handler;
	else if (context == SIGCONTEXT_HEREDOC)
		handler = signal_heredoc_handler;
	sigemptyset(&s_sigaction.sa_mask);
	s_sigaction.sa_handler = handler;
	s_sigaction.sa_flags = SA_RESTART;
	sigaction(SIGINT, &s_sigaction, NULL);
	sigaction(SIGQUIT, &s_sigaction, NULL);
}

static void	signal_prompt_handler(int sig)
{
	g_signal_status = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_redisplay();
		if (DEBUG == 13)
			ft_putstr_fd("minishell: SIGQUIT RECEIVED\n", 2);
	}
}

static void	signal_parent_handler(int sig)
{
	g_signal_status = sig;
	if (sig == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", 2);
}

static void	signal_heredoc_handler(int sig)
{
	g_signal_status = sig;

	rl_replace_line("word",0);
	rl_redisplay();
	rl_done = 1;
}
