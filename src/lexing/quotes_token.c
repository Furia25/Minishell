/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:49:57 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/17 20:06:56 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
size_t	index_last_closed_par(char *str, t_minishell *data);

static t_lst	*create_set_quote_node(char *str, t_lexeme_type type ,size_t len, t_minishell *data)
{
	char *node_lexeme;
	t_lst	*new_node;

	node_lexeme = ft_substr(str, 1, len - 1);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	new_node->type = type;
	if (ft_strchr("|&;()<> \t", str[len + 1]) == NULL)
		new_node->metacharacter_after = false;
	else
		new_node->metacharacter_after = true;
	return (new_node);
}

size_t single_quote_token(t_lst **tokens, char *str, t_minishell *data)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (str[i] != '\0' && str[i] != '\'')
	{
		if (str[i] == '*')
			str[i] = -'*';
		i++;
	}
	if (str[i] == '\0')
	{
		not_interpret_chara('\'', "\' (unclosed quote)", data);
		return (0);
	}
	lstadd_back(tokens, create_set_quote_node(str, SINGLE_Q, i, data));
	return (i + 1);
}

size_t double_quote_token(t_lst **tokens, char *str, t_minishell *data)
{
	size_t	i;
	size_t	i_last_closed_par;

	i = 1;
	while (str[i] != '\0' && str[i] != '\"')
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			i++;
			i_last_closed_par = index_last_closed_par(str + i, data);
			if (i_last_closed_par == 0)
				return (0);
			i = i + i_last_closed_par;
		}
		if (str[i] == '*' || str[i] == '?')
			str[i] = -str[i];
		i++;
	}
	if (str[i] == '\0')
	{
		not_interpret_chara('\"', "\' (unclosed quote)", data);
		return (0);
	}
	lstadd_back(tokens, create_set_quote_node(str, DOUBLE_Q, i, data));
	return (i + 1);
}

void	fusion_quote_token(t_lst *tokens, t_minishell *data)
{
	t_lst *buff;

	while (tokens)
	{
		if (tokens->next == NULL)
			return ;
		if ((tokens->type == WORD || tokens->type == DOLLAR
			|| tokens->type == SINGLE_Q || tokens->type == DOUBLE_Q)
			&& tokens->metacharacter_after == false)
		{
			if (tokens->next->type != DOLLAR)
			{
				tokens->lexeme = ft_strjoin_alt_gc(tokens->lexeme, tokens->next->lexeme,
					FREE_PARAM1, data);
				tokens->type = tokens->next->type;
				tokens->metacharacter_after = tokens->next->metacharacter_after;	
				check_malloc(tokens->lexeme, data);
				buff = tokens->next->next;
				gc_free_node(tokens->next, data);
				tokens->next = buff;
				continue ;
			}
		}
		tokens = tokens->next;
	}
}
