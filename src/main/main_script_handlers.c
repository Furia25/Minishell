/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_script_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:04:21 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/18 12:34:00 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	script_init(char **argv, t_minishell *data);
static bool	is_valid_script_line(char *line);

void	handle_script(char **argv, t_minishell *data)
{
	t_gnl_result	gnl;

	script_init(argv, data);
	gnl = get_next_line(data->script_fd);
	data->line++;
	while (gnl.line)
	{
		gc_add(gnl.line, data);
		if (!is_valid_script_line(gnl.line))
		{
			ft_putstr_fd(data->script_file, 2);
			ft_putstr_fd(": line ", 2);
			ft_putnbr_fd(data->line, 2);
			ft_putstr_fd(": invalid line exiting.\n", 2);
			break;
		}
		parsing_exec(gnl.line, data);
		gc_free(gnl.line, data);
		gnl = get_next_line(data->script_fd);
	}
	gc_free(gnl.line, data);
	close(data->script_fd);
	if (gnl.error && gnl.line == NULL)
		malloc_error(data);	
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

static bool	is_valid_script_line(char *line)
{
	while (line && *line)
	{
		if (!ft_isascii(*line))
			return (false);
		if (*line == '\n')
			break ;
		line++;
	}
	if (*line != '\n')
		return (false);
	return (true);
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
