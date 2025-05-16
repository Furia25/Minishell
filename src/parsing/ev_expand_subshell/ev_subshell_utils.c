/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_subshell_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:36:15 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 23:43:24 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_blank_in_end(char *str)
{
	size_t	i;

	if (str[0] == '\0')
		return ;
	i = ft_strlen(str) - 1;
	while (ft_strchr("\n\t ", str[i]) != NULL)
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

void check_blank_in_extremity(t_lst *token, t_minishell *data)
{
	char	*old_lexeme;

	old_lexeme = token->lexeme;
	if (ft_strchr("\n\t ", old_lexeme[0]) != NULL)
		token->type = DOLLAR;
	if (ft_strchr("\n\t ", old_lexeme[ft_strlen(old_lexeme) - 1]) != NULL)
		token->metacharacter_after = true;
	token->lexeme = ft_strtrim(old_lexeme, "\n\t ");
	check_malloc(token->lexeme, data);
	gc_free(old_lexeme, data);
}
