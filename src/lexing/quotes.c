/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:49:57 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/21 02:17:14 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	not_interpret_chara(char chara, char *str);

size_t single_quote_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (str[i] != '\0' && str[i] != '\'')
		i++;
	if (str[i] == '\0')
		not_interpret_chara('\'', "\' (unclosed single quote)");
	node_lexeme = ft_substr(str, 1, i - 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = SINGLE_Q;
	if (ft_strchr("|&;()<> \t", str[i + 1]) != NULL)
		new_node->metacharacter_after = true;
	else
		new_node->metacharacter_after = false;
	lstadd_back(tokens, new_node);
	return (i + 1);
}

size_t double_quote_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (str[i] != '\0' && str[i] != '\"')
		i++;
	if (str[i] == '\0')
		not_interpret_chara('\"', "\' (unclosed double quote)");
	node_lexeme = ft_substr(str, 1, i - 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = DOUBLE_Q;
	if (ft_strchr("|&;()<> ", str[i + 1]) != NULL)
		new_node->metacharacter_after = true;
	else
		new_node->metacharacter_after = false;
	lstadd_back(tokens, new_node);
	return (i + 1);
}

void	fusion_quote_token(t_lst *tokens)
{
	t_lst *buff;

	while (tokens)
	{
		if (tokens->next == NULL)
			return ;
		if ((tokens->type == WORD || tokens->type == SUBSHELL
			|| tokens->type == SINGLE_Q || tokens->type == DOUBLE_Q)
			&& tokens->metacharacter_after == false)
		{
			if (tokens->next->type == WORD || tokens->next->type == SINGLE_Q
					|| tokens->next->type == DOUBLE_Q)
			{
				tokens->lexeme = ft_strjoin_alt(tokens->lexeme, tokens->next->lexeme,
					FREE_PARAM1);
				buff = tokens->next->next;
				lstdelone(tokens->next, free);
				tokens->next = buff;
				continue ;
			}
		}
		tokens = tokens->next;
	}
}
