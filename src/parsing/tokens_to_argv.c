/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_argv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:18:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/23 15:22:12 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "debug.h"

size_t	tab_size(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	return (i);
}

static size_t	lst_size_alt(t_lst *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		if (lst->type == DOLLAR)
		{
			if (*(lst->lexeme) != '\0')
				i++;
		}
		else
			i++;
		lst = lst->next;
	}
	return (i);
}

char	**tokens_to_argv(t_lst *tokens, t_minishell *data)
{
	char	**argv;
	size_t	tokens_size;
	size_t	i;

	tokens_size = lst_size_alt(tokens);
	if (tokens_size == 0)
		return (NULL);
	argv = malloc((tokens_size + 1) * sizeof(char *));
	check_malloc(argv, data);
	argv[tokens_size] = NULL;
	i = 0;
	while (i < tokens_size)
	{
		if (*(tokens->lexeme) != '\0' || tokens->type != DOLLAR)
		{
			argv[i] = tokens->lexeme;
			i++;
		}
		tokens = tokens->next;
	}
	print_debug_argv(argv, 11,
		"\ndisplay argv after creating it\n");
	return (argv);
}
