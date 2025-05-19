/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_shell_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 01:51:08 by val               #+#    #+#             */
/*   Updated: 2025/05/19 21:10:38 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errno.h"
#include "minishell_signal.h"
#include "minishell.h"

static bool	is_void_or_full_blank(char *input, t_minishell *data);
static char	*get_prompt(t_minishell *data);

void	handle_shell(t_minishell *data)
{
	char	*input;
	char	*prompt;

	disable_echoctl();
	while (1)
	{
		setup_signals(SIGCONTEXT_PROMPT);
		prompt = get_prompt(data);
		input = readline(prompt);
		gc_add(input, data);
		gc_free(prompt, data);
		if (!input)
		{
			if (errno != 0)
				raise_error(data);
			exit_minishell(data);
			return ;
		}
		if (is_void_or_full_blank(input, data) == false)
		{
			parsing_exec(input, data);
			add_history(input);
		}
		gc_free(input, data);
	}
}

static char	*get_prompt(t_minishell *data)
{
	t_envvar		*var;
	t_hash_entry	*temp_entry;
	char			*prompt;
	char			*temp_welcome;

	temp_entry = hashmap_search(hash(ENV_PWD), &data->environment);
	if (!temp_entry)
		return (ft_strdup(PROMPT));
	var = (t_envvar *) temp_entry->value;
	prompt = ft_strjoin(var->value, PROMPT);
	check_malloc(prompt, data);
	if (!data->welcome_start)
	{
		data->welcome_start = true;
		temp_welcome = ft_strjoin(MINISHELL_WELCOME_START, MINISHELL_NAME);
		check_malloc(temp_welcome, data);
		temp_welcome = ft_strjoin_alt_gc(temp_welcome, 
			MINISHELL_WELCOME_END, FREE_PARAM1, data);
		check_malloc(temp_welcome, data);
		prompt = ft_strjoin(temp_welcome, prompt);
		check_malloc(prompt, data);
		gc_free(temp_welcome, data);
	}
	return (prompt);
}

static bool	is_void_or_full_blank(char *input, t_minishell *data)
{
	size_t	i;

	i = 0;
	if (input[i] == '\0')
		return (true);
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\0')
		data->exit_code = 0;
	return (input[i] == '\0');
}
