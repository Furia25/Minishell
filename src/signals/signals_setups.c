/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setups.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:54:33 by val               #+#    #+#             */
/*   Updated: 2025/05/29 15:46:58 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"
#include "debug.h"

volatile sig_atomic_t	g_signal_status = 0;

static void	signal_prompt_handler(int sig);
static void	signal_parent_handler(int sig);
static void	signal_heredoc_handler(int sig);

void	setup_signals(t_signal_context context)
{
	struct sigaction	s_sigaction;
	void				(*handler)(int);

	handler = NULL;
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
	if (rl_readline_state & RL_STATE_COMPLETING)
	{
		write(1, "\n", 1);
		rl_readline_state &= ~RL_STATE_COMPLETING;
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
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
			ft_printf_fd(STDERR_FILENO,
				"%s: SIGQUIT RECEIVED\n", MINISHELL_NAME);
	}
}

static void	signal_parent_handler(int sig)
{
	if (rl_readline_state & RL_STATE_COMPLETING)
		return ;
	g_signal_status = sig;
	if (sig == SIGQUIT)
		ft_printf_fd(STDERR_FILENO, "%s\n", SIGNAL_CORE_DUMP_MESSAGE);
}

static void	signal_heredoc_handler(int sig)
{
	g_signal_status = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	if (sig == SIGINT)
		rl_done = 1;
}
