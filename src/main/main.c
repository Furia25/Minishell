/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/23 01:44:35 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "environment.h"
#include "garbage_collector.h"
#include <stdio.h>
#include <signal.h>

static int	init_minishell(t_minishell *data, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	int			flags;

	init_minishell(&data, envp);
	flags = check_flags_c(argc, argv);
	if (flags < 2)
	{
		if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO) || flags == 1)
			handle_script(argv, &data);
		else
		{
			if (!MINISHELL_SHOW_SIGNALS)
				disable_echoctl();
			handle_shell(&data);
		}
	}
	else
	{
		handle_cflag(argv + flags, &data);
	}
	exit_minishell(&data);
	return (EXIT_SUCCESS);
}

void	exit_minishell(t_minishell *data)
{
	if (data->in_child)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	rl_clear_history();
	hashmap_free_content(&data->environment);
	free_chartab(data->environment_tab);
	if (data->command_tab)
		close_all_fds(data->command_tab);
	if (data->script_fd != -1)
		close(data->script_fd);
	gc_full_clean(data);
	exit(data->exit_code);
}

static int	init_minishell(t_minishell *data, char **envp)
{
	data->exit_code = EXIT_SUCCESS;
	if (!hashmap_init_basics(&(data->environment), envvar_free))
		exit(EXIT_FAILURE);
	if (!env_populate(envp, &(data->environment)))
		raise_error(data);
	if (!gc_init(data))
		raise_error(data);
	data->welcome_start = false;
	data->command_tab = NULL;
	data->environment_tab = NULL;
	data->in_child = false;
	data->in_pipe = false;
	data->line = 1;
	data->script_mode = false;
	data->script_fd = -1;
	data->use_hard_path = true;
	return (1);
}
