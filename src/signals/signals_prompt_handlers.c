/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_prompt_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:53:41 by val               #+#    #+#             */
/*   Updated: 2025/05/18 13:57:32 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"

void	handle_prompt_sigint(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_prompt_sigquit(void)
{
	if (DEBUG == 13)
		ft_putstr_fd("minishell: SIGQUIT RECEIVED\n", 2);
}
