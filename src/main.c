/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/14 18:28:57 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "environment.h"
#include <stdio.h>

void	test_lstprint(void *str)
{
	ft_printf("%s ", (char *)str);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_wsearch	wildcard_result;
	t_minishell	data;

	(void) argc;
	(void) argv;
	if (!hashmap_init_basics(&data.environment, envvar_free))
		return (EXIT_FAILURE);
	if (!env_populate(envp, &data.environment))
	{
		hashmap_free_content(&data.environment);
		return (EXIT_FAILURE);
	}
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
				hashmap_free_content(&data.environment);
				return (0);
			}
		}
		if (*input)
		{
			if (ft_strcmp(input, "env") == 0)
			{
				env_builtin(&data);
			}else
			if (ft_strncmp(input, "export", 6) == 0)
			{
				export_builtin(2, (char *[]){"export", "TEST=GROSCACACACACACACACACACACACACACACACACACACACACACACACACACACACA", NULL}, &data);
			}else {
				add_history(input);
				wildcard_result = wildcard_lst_from_token(input);
				if (wildcard_result.code == -1)
					perror("WILDCARDS :");
				if (wildcard_result.code == 0 && wildcard_result.result == NULL)
					ft_printf("%s ", input);
				ft_lstiter(wildcard_result.result, &test_lstprint);
				ft_putchar_fd('\n', 1);
				ft_lstclear(&wildcard_result.result, free);
			}
			
		}
		free(input);
	}
}
