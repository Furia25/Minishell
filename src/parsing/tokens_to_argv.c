/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_argv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:18:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 05:29:10 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t lst_size(t_lst *lst)
{
	size_t	i;
	char	*lexeme;

	i = 0;
	while (lst)
	{
		lexeme = lst->lexeme;
		if (lst->type != DOUBLE_Q && lst->type != SINGLE_Q)
		{
			if (ft_strchr("\n\t ", *lexeme) == NULL)
				i++;
		}
		else
		{
			if (*lexeme != '\0')
				i++;
		}
		lst = lst->next;
	}
	return (i);
}

static int	set_str(char **str, char *new_str)
{
	*str = new_str;
	return (1);
}

char	**tokens_to_argv(t_lst *tokens, t_minishell *data)
{
	char	**argv;
	size_t	tokens_size;
	size_t	i;

	tokens_size = lst_size(tokens);
	argv = malloc((tokens_size + 1) * sizeof(char*));
	check_malloc(argv, data);
	argv[tokens_size] = NULL;
	i = 0;
	while (i < tokens_size)
	{
		if (tokens->type != DOUBLE_Q && tokens->type != SINGLE_Q)
		{
			if (ft_strchr("\n\t ", *(tokens->lexeme)) == NULL)
				i = i + set_str(&(argv[i]), tokens->lexeme);
		}
		else
		{
			if (*(tokens->lexeme) != '\0')
				i = i + set_str(&(argv[i]), tokens->lexeme);
		}
		tokens = tokens->next;
	}
	return (argv);
}

