/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:16:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 02:54:13 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errno.h"

void	print_basic_error(char *error_name)
{
	ft_printf_fd(STDERR_FILENO, "%s: %s: %s\n",
		MINISHELL_NAME, error_name, strerror(errno));
}

void	print_extended_error(char *error_name, char *cause, char *error)
{
	if (error_name)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: %s %s\n",
			MINISHELL_NAME, error_name, cause, error);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s %s\n",
			MINISHELL_NAME, cause, error);
	}
}
