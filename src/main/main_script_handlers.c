/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_script_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:04:21 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/15 18:37:49 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	script_init(char **argv, t_minishell *data);

void	handle_script(char **argv, t_minishell *data)
{
	t_gnl_result	gnl;

	script_init(argv, data);
	gnl = get_next_line(data->script_fd);
	while (gnl.line)
	{
		data->line++;
		parsing_exec(gnl.line, data);
		free(gnl.line);
		gnl = get_next_line(data->script_fd);
	}
	if (gnl.error)
		malloc_error(data);
	close(data->script_fd);
}
static void	script_init(char **argv, t_minishell *data)
{
	data->script_mode = true;
	data->script_file = argv[1];
	if (argv[1] == NULL)
		data->script_fd = 0;
	else
		data->script_fd = open(argv[1], O_RDONLY);
	if (data->script_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(argv[1]);
		data->exit_code = EXIT_FAILURE;
		exit_minishell(data);
		return ;
	}
}

void	handle_cflag(char **argv, t_minishell *data)
{
	data->script_mode = 1;
	data->script_file = "minishell";
	data->script_fd = -1;
	data->line = 1;
	while (*argv)
	{
		parsing_exec(*argv, data);
		data->line += 1;
		argv++;
	}
}
