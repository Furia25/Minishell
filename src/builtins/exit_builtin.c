/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:22:24 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/16 18:45:22 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_builtin(int argc, char **argv, t_minishell *data)
{
	if (argc > 2)
	{
		ft_putstr_fd("minishell", 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(BUILTIN_ERROR_EXIT, 2);
		ft_putchar_fd('\n', 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
