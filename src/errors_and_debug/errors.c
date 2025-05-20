/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:17:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/21 00:41:01 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "minishell.h"
#include "garbage_collector.h"

void	raise_error(t_minishell *data)
{
	perror(MINISHELL_NAME);
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	raise_error_category(char *error_category, t_minishell *data)
{
	print_basic_error(error_category);
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	command_notfound(char *cmd, t_minishell *data)
{
	char	*temp_file;
	char	*temp_cmd;

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if (is_directory(cmd))
	{
		data->exit_code = 126;
		exit_minishell(data);
	}
	if (cmd)
		temp_cmd = cmd;
	else
		temp_cmd = "UNKNOWN";
	if (data->script_mode)
	{
		if (data->script_file)
			temp_file = data->script_file;
		else
			temp_file = MINISHELL_NAME;
		ft_printf_fd(2, "%s: line %d: \"%s\" command not found\n",
			temp_file, data->line, temp_cmd);
	}
	else
		ft_printf_fd(2, "%s: \"%s\" command not found\n", MINISHELL_NAME, temp_cmd);
	exit_minishell(data);
}
