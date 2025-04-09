/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/09 02:08:18 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include <stdio.h>

void	test_lstprint(void *str)
{
	ft_printf("%s ", (char *)str);
}

int	main(void)
{
	char		*input;
	t_wsearch	wildcard_result;
	
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			free(input);
			rl_clear_history();
			if (DEBUG == 2 || DEBUG == 1)
			{
				ft_putstr_fd("\nEnd of program (EOF detected), history is cleaned\n", 2);
				return (0);
			}
		}
		if (*input)
		{
			add_history(input);
			wildcard_result = wildcard_lst_from_token(input);
			if (wildcard_result.code == -1)
				perror("WILDCARDS :");
			if (wildcard_result.code == 0 && wildcard_result.result == NULL)
				ft_printf("%s ", input);
			ft_lstiter(wildcard_result.result, &test_lstprint);
			ft_lstclear(&wildcard_result.result, free);
		}
		free(input);
	}
}