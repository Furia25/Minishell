/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/29 19:31:15 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "environment.h"
#include "garbage_collector.h"
#include <stdio.h>

static int	init_minishell(t_minishell *data, char **envp);
int			check_flags_c(int argc, char **argv);
static void	handle_subshell(int argc, char **argv, t_minishell *data);
static void	handle_shell(int argc, char **argv, t_minishell *data);

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_minishell	data;
	int			flags;

	init_minishell(&data, envp);
	if (isatty(0) == 0)
	{
		flags = check_flag_c(argv);
		if (flags == -1)
			handle_script(argc, argv, &data);
		else
			handle_subshell(argc, argv, &data);
	}
	else
	{
		handle_shell(argc, argv, &data);
	}
	init_minishell(&data, envp);
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
	return (1);
}

static void	handle_subshell(int argc, char **argv, t_minishell *data)
{
	
}

static void	handle_shell(int argc, char **argv, t_minishell *data)
{
	char	*input;

	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			free(input);
			rl_clear_history();
			if (DEBUG == 2 || DEBUG == 1)
			{
				ft_putstr_fd("\nEnd of program (EOF detected), \
					history is cleaned\n", 2);
				exit_minishell(&data);
				return (0);
			}
		}
		if (*input && input)
		{
		
		}
		free(input);
	}
}

int	check_flags_c(int argc, char **argv)
{
	int		index;
	char	*temp;
	
	if (argc == 1)
		return (-1);
	index = 1;
	while (argv[index] && index < argc)
	{
		temp = argv[index];
		if (*temp != '-')
			return (-1);
		while (*temp && temp)
		{
			if (*temp != 'c')
				return (-1);
			temp++;
		}
		index++;
	}
	return (index);
}
