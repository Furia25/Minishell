/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/15 18:23:52 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "environment.h"
#include "garbage_collector.h"
#include <stdio.h>

static int	init_minishell(t_minishell *data, char **envp);
static void	handle_shell(t_minishell *data);
static bool	is_void_or_full_blank(char *input);

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
		handle_cflag(argv + flags, &data);
	}
	exit_minishell(&data);
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
	data->command_tab = NULL;
	data->environment_tab = NULL;
	data->in_child = false;
	data->in_pipe = false;
	data->line = 0;
	data->script_mode = false;
	data->script_fd = -1;
	data->is_subshell = false;
	return (1);
}

static char	*get_prompt(t_minishell *data);

static void	handle_shell(t_minishell *data)
{
	char	*input;
	char	*prompt;

	while (1)
	{
		prompt = get_prompt(data);
		if (!prompt)
			exit_minishell(data);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			free(input);
			rl_clear_history();
			ft_putstr_fd("End of program (EOF detected)\n", 2);
			exit_minishell(data);
			return ;
		}
		if (is_void_or_full_blank(input) == false)
		{
			parsing_exec(input, data);
			add_history(input);
		}
		free(input);
	}
}

static char	*get_prompt(t_minishell *data)
{
	t_envvar		*var;
	t_hash_entry	*temp_entry;
	char			*prompt;

	temp_entry = hashmap_search(hash(ENV_PWD), &data->environment);
	if (!temp_entry)
		return (ft_strdup(PROMPT));
	var = (t_envvar *) temp_entry->value;
	prompt = ft_strjoin(var->value, PROMPT);
	return (prompt);
}

static bool	is_void_or_full_blank(char *input)
{
	size_t	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (input[i] == '\0');
}
