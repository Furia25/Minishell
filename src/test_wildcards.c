/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wildcards.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:57:12 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/11 20:03:04 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;
	init_minishell(&data, envp);
	char	*input = malloc(4);

	input[0] = 'k';
	input[1] = '*';
	input[2] = '*';
	input[3] = '\0';
	t_wsearch wildcard_result = wildcard_lst_from_token(input);
	if (wildcard_result.code == -1)
		perror("WILDCARDS :");
	if (wildcard_result.code == 0 && wildcard_result.result == NULL)
		ft_printf("%s ", input);
	while (wildcard_result.result)
	{
		ft_printf("%s ", (char*)wildcard_result.result->content);
		wildcard_result.result = wildcard_result.result->next;
	}
	ft_putchar_fd('\n', 1);
}
