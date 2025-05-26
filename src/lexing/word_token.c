/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:34:55 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/26 20:14:47 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "debug.h"

ssize_t	in_parentheses_len(char *str, t_minishell *data)
{
	ssize_t	i;
	ssize_t	par_number;

	i = 0;
	par_number = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ')')
		{
			par_number--;
			if (par_number == 0)
			return (i);
		}
		if (str[i] == '(')
			par_number++;
		i++;
	}
	not_interpret_chara('(', "\' (invalid parenthesis)", data);
	return (-1);
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
	ssize_t	in_par_len;

	i = 0;
	while (ft_strchr("|&;<>() \t\'\"", str[i]) == NULL)
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			i++;
			in_par_len = in_parentheses_len(str + i, data);
			if (in_par_len == -1)
				return (0);
			i = i + in_par_len;
		}
		i++;
	}
	lstadd_back(tokens, create_set_word_node(str, i, data));
	return (i);
}
