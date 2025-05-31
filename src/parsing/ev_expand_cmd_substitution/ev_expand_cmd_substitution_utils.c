/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_cmd_substitution_utils.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:36:15 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/31 20:59:57 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_nl_in_end(char *str)
{
	size_t	i;

	if (str[0] == '\0')
		return ;
	i = ft_strlen(str) - 1;
	while (str[i] == '\n')
	{
		if (i == 0)
		{
			str[0] = '\0';
			return ;
		}
		i--;
	}
	str[i + 1] = '\0';
}

t_lst	*del_node_empty_lexeme(t_lst *prev, t_lst *current, t_leaf *cmd,
		t_minishell *data)
{
	if (prev == NULL)
	{
		cmd->tokens = current->next;
		gc_free_node(current, data);
		return (cmd->tokens);
	}
	prev->next = current->next;
	gc_free_node(current, data);
	return (prev->next);
}

void	check_blank_in_extremity(t_lst *token, t_minishell *data)
{
	char	*old_lexeme;

	old_lexeme = token->lexeme;
	if (ft_strchr(BASIC_WHITESPACE, old_lexeme[0]) != NULL)
		token->type = DOLLAR;
	if (ft_strchr(BLANK, old_lexeme[ft_strlen(old_lexeme) - 1]) != NULL)
		token->metacharacter_after = true;
	token->lexeme = ft_strtrim(old_lexeme, BASIC_WHITESPACE);
	check_malloc(token->lexeme, data);
	gc_free(old_lexeme, data);
}
