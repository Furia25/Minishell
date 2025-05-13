/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/13 15:30:37 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "environment.h"
#include "garbage_collector.h"
#include <stdio.h>

static int	init_minishell(t_minishell *data, char **envp);
static int	check_flags_c(int argc, char **argv);
static void	handle_script(char **argv, t_minishell *data);
// static void	handle_subshell(int argc, char **argv, t_minishell *data);
static void	handle_shell(t_minishell *data);
void	parsing_exec(char *input, t_minishell *data);

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	int			flags;

	init_minishell(&data, envp);
	flags = check_flags_c(argc, argv);
	if (flags < 2)
	{
		if (isatty(0) == 0 || flags == 1)
			handle_script(argv, &data);
		else
			handle_shell(&data);
	}
	else
	{
		// handle_subshell(argc, argv, &data);
	}
	return (data.exit_code);
}

static int	init_minishell(t_minishell *data, char **envp)
{
	data->exit_code = EXIT_SUCCESS;
	if (!hashmap_init_basics(&(data->environment), envvar_free))
		exit(EXIT_FAILURE);
	if (!env_populate(envp, &(data->environment)))
		malloc_error(data);
	if (!gc_init(data))
		malloc_error(data);
	data->in_child = false;
	data->in_pipe = false;
	data->line = 0;
	data->script_mode = false;
	data->script_fd = -1;
	return (1);
}

static void	handle_script(char **argv, t_minishell *data)
{
	t_gnl_result	gnl;

	data->script_mode = true;
	data->script_file = argv[1];
	data->script_fd = open(argv[1], O_RDONLY);
	if (data->script_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(argv[1]);
		data->exit_code = EXIT_FAILURE;
		exit_minishell(data);
		return ;
	}
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

// static void	handle_subshell(int argc, char **argv, t_minishell *data)
// {

// }

static void	handle_shell(t_minishell *data)
{
	char	*input;

	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			free(input);
			// rl_clear_history();
			ft_putstr_fd("End of program (EOF detected)\n", 2);
			exit_minishell(data);
			return ;
		}
		if (input && *input)
			parsing_exec(input, data);
		free(input);
	}
}

static int	check_flags_c(int argc, char **argv)
{
	int		index;
	char	*temp;

	if (argc == 1)
		return (0);
	index = 1;
	while (argv[index])
	{
		temp = argv[index];
		if (*temp != '-')
			return (index);
		temp++;
		while (*temp && temp)
		{
			if (*temp != 'c')
				return (index);
			temp++;
		}
		index++;
	}
	return (index);
}
