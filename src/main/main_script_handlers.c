/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_script_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:04:21 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 03:55:23 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	script_init(char **argv, t_minishell *data);
static bool	is_valid_script_line(char *line, bool ended);

void	handle_script(char **argv, t_minishell *data)
{
	t_gnl_result	gnl;

	script_init(argv, data);
	gnl = get_next_line(data->script_fd);
	while (gnl.line && !gnl.error)
	{
		gc_add(gnl.line, data);
		if (!is_valid_script_line(gnl.line, gnl.ended))
		{
			ft_printf_fd(STDERR_FILENO, "%s: line %d: invalid line exiting.\n",
				data->script_file, data->line);
			break ;
		}
		if (gnl.line[0] != '\n')
			parsing_exec(gnl.line, data);
		gc_free(gnl.line, data);
		gnl = get_next_line(data->script_fd);
	}
	gc_free(gnl.line, data);
	close(data->script_fd);
	if (gnl.error)
		raise_error(data);
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
		print_basic_error(argv[1]);
		data->exit_code = EXIT_FAILURE;
		exit_minishell(data);
		return ;
	}
}

static bool	is_valid_script_line(char *line, bool ended)
{
	unsigned char	c;

	while (*line && *line != '\n')
	{
		c = (unsigned char)*line;
		if (c < 0x20 && c != 0x09 && c != 0x0A && c != 0x0D)
			return (false);
		line++;
	}
	return (*line == '\n' || ended);
}

void	handle_cflag(char **argv, t_minishell *data)
{
	data->script_mode = 1;
	data->script_file = MINISHELL_NAME;
	data->script_fd = -1;
	if (!(*argv))
	{
		data->exit_code = 2;
		print_extended_error(NULL, "-c",
			"option requires at least one argument");
		exit_minishell(data);
	}
	while (*argv)
	{
		parsing_exec(*argv, data);
		argv++;
	}
}
