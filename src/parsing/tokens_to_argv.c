/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_argv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:18:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/18 18:04:58 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t lst_size(t_lst *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

char	**tokens_to_argv(t_lst *tokens)
{
	t_lst	*temp;
	char	**argv;
	size_t	tokens_size;
	size_t	i;

	tokens_size = lst_size(tokens);
	argv = malloc((tokens_size + 1) * sizeof(char*));

	i = 0;
	while (i < tokens_size)
	{
		argv[i] = tokens->lexeme;
		tokens = tokens->next;
		i++;
	}
	argv[tokens_size] = NULL;
	return (argv);
}

/* int	main(void)
{
	t_lst	*tokens;
	t_lst	*temp;
	char *input = "aaaaa bbbb xccs <> |";
	char	**argv;


	tokens = NULL;
	create_tokens(&tokens, input);
	fusion_quote_token(tokens);
	argv = tokens_to_argv(tokens);
	while (*argv)
	{
		ft_printf("str: %s\n", *argv);
		argv++;
	}
} */
