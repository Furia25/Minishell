/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:34:55 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 04:03:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	index_last_closed_par(char *str, t_minishell *data)
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
		not_interpret_chara('(', "\' (unclosed parenthesis)", data);
		return (0);
	}
	return (index_last_closed_par);
}

static void	check_metachara_after(char c, t_lst *token)
{
	if (c == '\'' || c == '\"')
		token->metacharacter_after = false;
	else
		token->metacharacter_after = true;
}

size_t word_token(t_lst **tokens, char *str, t_minishell *data)
{
	size_t	i;
	size_t	i_last_closed_par;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (ft_strchr("|&;<>() \t\'\"", str[i]) == NULL)
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			i_last_closed_par = index_last_closed_par(str + i, data);
			if (i_last_closed_par == 0)
				return (0);
			i = i + i_last_closed_par;
		}
		i++;
	}
	node_lexeme = ft_substr(str, 0, i);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	new_node->type = WORD;
	check_metachara_after(str[i], new_node);
	lstadd_back(tokens, new_node);
	return (i);
}
