/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:17:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 16:09:38 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "environment.h"
#include "garbage_collector.h"
#include <stdio.h>

void	test_lstprint(void *str)
{
	ft_printf("%s ", (char *)str);
}

int	init_minishell(t_minishell *data, char **envp)
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

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_wsearch	wildcard_result;
	t_minishell	data;

	(void) argc;
	(void) argv;
	init_minishell(&data, envp);
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
		if (*input)
		{
			if (ft_strcmp(input, "env") == 0)
			{
				env_builtin(&data);
			}else
			if (ft_strncmp(input, "export", 6) == 0)
			{
				export_builtin(2, (char *[]){"export", "HOME=./test_home", NULL}, &data);
				export_builtin(2, (char *[]){"export", "TEST", NULL}, &data);
				export_builtin(2, (char *[]){"export", "TEST2=", NULL}, &data);
				export_builtin(1, (char *[]){"export", NULL}, &data);
			}else 
			if (ft_strncmp(input, "unset", 5) == 0)
			{
				unset_builtin(2, (char *[]){"unset", "PATH", "TEST", NULL}, &data);
			}else
			if (ft_strcmp(input, "pwd") == 0)
			{
				pwd_builtin();
			}else
			if (ft_strncmp(input, "echo", 4) == 0)
			{
				echo_builtin(3, (char *[]){"echo", "-n", "bonjour", NULL});
			}else
			if (ft_strcmp(input, "cd") == 0)
			{
				cd_builtin(2, (char *[]){"cd", "-", NULL}, &data);
			}else if (ft_strcmp(input, "export") == 0)
			{
				export_builtin(2, (char *[]){"export", "HOME=./test_home", NULL}, &data);
				export_builtin(2, (char *[]){"export", "TEST", NULL}, &data);
				export_builtin(2, (char *[]){"export", "TEST2=", NULL}, &data);
				export_builtin(1, (char *[]){"export", NULL}, &data);
			}else if (ft_strncmp(input, "unset", 5) == 0) {
				unset_builtin(2, (char *[]){"unset", "PATH", "TEST", NULL}, &data);
			}else
			if (ft_strcmp(input, "pwd") == 0)
			{
				pwd_builtin();
			}else
			if (ft_strncmp(input, "echo", 4) == 0)
			{
				echo_builtin(3, (char *[]){"echo", "-n", "bonjour", NULL});
			}else
			if (ft_strcmp(input, "cd") == 0)
			{
				cd_builtin(2, (char *[]){"cd", "-", NULL}, &data);
			}else
			{
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