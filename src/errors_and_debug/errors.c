/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:17:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 17:48:36 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	malloc_error(t_minishell *data)
{
	perror("minishell");
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	open_error(t_minishell *data)
{
	ft_putstr_fd("minishell", 2);
	ft_putstr_fd(": ", 2);
	perror("open");
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	pipe_error(t_minishell *data)
{
	ft_putstr_fd("minishell", 2);
	ft_putstr_fd(": ", 2);
	perror("pipe");
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	fork_error(t_minishell *data)
{
	ft_putstr_fd("minishell", 2);
	ft_putstr_fd(": ", 2);
	perror("fork");
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	command_notfound(char *cmd, t_minishell *data)
{
	if (data->script_mode)
	{
		if (data->script_file)
			ft_putstr_fd(data->script_file, 2);
		else
			ft_putstr_fd("minishell", 2);
		ft_putstr_fd(": line ", 2);
		ft_putnbr_fd(data->line, 2);
		ft_putstr_fd(": ", 2);
	}
	else
		ft_putstr_fd("minishell: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	else
		ft_putstr_fd("UNKNOW", 2);
	ft_putstr_fd(" : command not found\n", 2);
	data->exit_code = 127;
	exit_minishell(data);
}
