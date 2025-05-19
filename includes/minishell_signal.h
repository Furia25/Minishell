/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:34:15 by val               #+#    #+#             */
/*   Updated: 2025/05/19 02:19:53 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H
# define _POSIX_C_SOURCE  200809L
# include <signal.h>
# include <sys/types.h>

extern volatile sig_atomic_t	g_signal_status;

typedef enum e_signal_context
{
	SIGCONTEXT_PROMPT,
	SIGCONTEXT_HEREDOC,
	SIGCONTEXT_FORK,
	SIGCONTEXT_PARENT
}	t_signal_context;

void	setup_signals(t_signal_context context);
#endif