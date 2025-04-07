/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:49:57 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/08 00:05:50 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t single_quote_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (ft_isprint(str[i]) != 0 && str[i] != '\'')
		i++;
	node_lexeme = ft_substr(str, 1, i - 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = SINGLE_Q;
	if (ft_strchr("|&;()<> ", str[i + 1]) != NULL)
		new_node->metacharacter_after = YES;
	else
		new_node->metacharacter_after = NO;
	lstadd_back(tokens, new_node);
	return (i + 1);

}

size_t double_quote_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (ft_isprint(str[i]) != 0 && str[i] != '\"')
		i++;
	node_lexeme = ft_substr(str, 1, i - 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = DOUBLE_Q;
	if (ft_strchr("|&;()<> ", str[i + 1]) != NULL)
		new_node->metacharacter_after = YES;
	else
		new_node->metacharacter_after = NO;
	lstadd_back(tokens, new_node);
	return (i + 1);
}

void	fusion_quote_token(t_lst *tokens)
{
	t_lst *temp;

	temp = tokens;
	while (temp) 
	{
		if ((temp->type == SINGLE_Q || temp->type == DOUBLE_Q)
			&& temp->metacharacter_after == NO)
		{
			temp->lexeme = ft_strjoin_alt(temp->lexeme, temp->next->lexeme,
				FREE_PARAM1);
			lstdelone(temp->next, free);
			temp->next = temp->next->next;
		}
		temp = temp->next;
	}
}


