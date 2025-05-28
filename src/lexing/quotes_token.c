/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:49:57 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/28 17:29:24 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "debug.h"

static t_lst	*create_set_quote_node(char *str, t_lexeme_type type,
	size_t len, t_minishell *data)
{
	char	*node_lexeme;
	t_lst	*new_node;

	node_lexeme = ft_substr(str, 1, len - 1);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	while (*node_lexeme != '\0')
	{
		if (*node_lexeme == '*' || *node_lexeme == '?')
			*node_lexeme *= -1;
		node_lexeme++;
	}
	new_node->type = type;
	if (ft_strchr(METACHARACTER, str[len + 1]) == NULL)
		new_node->metacharacter_after = false;
	else
		new_node->metacharacter_after = true;
	return (new_node);
}

ssize_t	single_quote_token(t_lst **tokens, char *str, t_minishell *data)
{
	ssize_t	i;

	i = 1;
	while (str[i] != '\0' && str[i] != '\'')
		i++;
	if (str[i] == '\0')
	{
		not_interpret_chara('\'', "\' (unclosed quote)", data);
		return (-1);
	}
	lstadd_back(tokens, create_set_quote_node(str, SINGLE_Q, i, data));
	return (i + 1);
}

ssize_t	double_quote_token(t_lst **tokens, char *str, t_minishell *data)
{
	ssize_t	i;
	ssize_t	in_par_len;

	i = 1;
	while (str[i] != '\0' && str[i] != '\"')
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
	if (str[i] == '\0')
	{
		not_interpret_chara('\"', "\' (unclosed quote)", data);
		return (-1);
	}
	lstadd_back(tokens, create_set_quote_node(str, DOUBLE_Q, i, data));
	return (i + 1);
}
