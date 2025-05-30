/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:34:15 by val               #+#    #+#             */
/*   Updated: 2025/05/23 15:07:20 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H
# define _POSIX_C_SOURCE  200809L
# include <signal.h>
# include <sys/types.h>

# define SIGNAL_CORE_DUMP_MESSAGE	"Quit (core dumped)"

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