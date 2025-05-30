/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:33:16 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 01:38:44 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

ssize_t	in_parentheses_len(char *str)
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
				return (i - 1);
		}
		if (str[i] == '(')
			par_number++;
		i++;
	}
	return (-1);
}

t_lst	*matching_parenthesis(t_lst *tokens)
{
	ssize_t	par_number;

	par_number = 0;
	while (tokens != NULL)
	{
		if (tokens->type == PAR_CLOSE)
			par_number--;
		if (tokens->type == PAR_OPEN)
			par_number++;
		if (par_number == 0)
			break ;
		tokens = tokens->next;
	}
	return (tokens);
}

bool	are_parentheses_empty(t_lst *tokens)
{
	if (tokens->next != NULL
		&& tokens->type == PAR_OPEN && tokens->next->type == PAR_CLOSE)
		return (true);
	return (false);
}

bool	have_one_pair_of_parentheses(t_lst *tokens)
{
	while (tokens)
	{
		if (tokens->type == PIPE || tokens->type == OR || tokens->type == AND)
			return (true);
		if (tokens->type == PAR_OPEN)
			return (false);
		tokens = tokens->next;
	}
	return (true);
}
