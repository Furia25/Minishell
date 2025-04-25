/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:34:55 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/25 03:29:35 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	check_par_subshell(char *str, size_t index, t_minishell *data)
{
	if (str[index] == '$')
	{
		index++;
		while (str[index] != '\0' && str[index] != ')')
			index++;
		if (str[index] == '\0')
			not_interpret_chara('(', "\' (unclosed parenthesis)", data);
	}
	return (index);
}

size_t word_token(t_lst **tokens, char *str, t_minishell *data)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (ft_strchr("|&;<>() \t\'\"", str[i]) == NULL)
	{
		check_par_subshell(str, i, data);
		i++;
	}
	node_lexeme = ft_substr(str, 0, i);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	new_node->type = WORD;
	if (ft_strchr("|&;()<> \t", str[i]) != NULL)
		new_node->metacharacter_after = true;
	else
		new_node->metacharacter_after = false;
	lstadd_back(tokens, new_node);
	return (i);
}
