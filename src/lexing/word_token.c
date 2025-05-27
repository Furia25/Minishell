/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:34:55 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/27 18:34:17 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "debug.h"

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

ssize_t	word_token(t_lst **tokens, char *str, t_minishell *data)
{
	ssize_t	i;
	ssize_t	in_par_len;

	i = 0;
	while (ft_strchr("|&;<>() \t\'\"", str[i]) == NULL)
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			in_par_len = in_parentheses_len(str + i + 1);
			if (in_par_len == -1)
			{
				not_interpret_chara('(', "\' (invalid parenthesis)", data);
				return (-1);
			}
			i = i + 1 + in_par_len + 1;
		}
		i++;
	}
	lstadd_back(tokens, create_set_word_node(str, i, data));
	return (i);
}
