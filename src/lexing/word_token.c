/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:34:55 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/22 18:24:33 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	index_last_closed_par(char *str, t_minishell *data)
{
	size_t	i;
	size_t	index_last_closed_par;

	i = 0;
	index_last_closed_par = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ')')
			index_last_closed_par = i;
		i++;
	}
	if (index_last_closed_par == 0)
	{
		not_interpret_chara('(', "\' (invalid parenthesis)", data);
		return (0);
	}
	return (index_last_closed_par);
}

static t_lst	*create_set_word_node(char *str, size_t len, t_minishell *data)
{
	char	*node_lexeme;
	t_lst	*new_node;

	node_lexeme = ft_substr(str, 0, len);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	new_node->type = WORD;
	if (str[len] == '\'' || str[len] == '\"')
		new_node->metacharacter_after = false;
	else
		new_node->metacharacter_after = true;
	return (new_node);
}

size_t	word_token(t_lst **tokens, char *str, t_minishell *data)
{
	size_t	i;
	size_t	i_last_closed_par;

	i = 0;
	while (ft_strchr("|&;<>() \t\'\"", str[i]) == NULL)
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			i++;
			i_last_closed_par = index_last_closed_par(str + i, data);
			if (i_last_closed_par == 0)
				return (0);
			i = i + i_last_closed_par;
		}
		i++;
	}
	lstadd_back(tokens, create_set_word_node(str, i, data));
	return (i);
}
