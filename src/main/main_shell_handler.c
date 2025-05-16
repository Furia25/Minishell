/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_shell_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 01:51:08 by val               #+#    #+#             */
/*   Updated: 2025/05/17 01:52:03 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_void_or_full_blank(char *input);
static char	*get_prompt(t_minishell *data);

void	handle_shell(t_minishell *data)
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
