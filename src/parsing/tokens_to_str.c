/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:18:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/21 02:06:33 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void append_lexeme_to_string(t_lexeme_type type, char **str,
			char *lexeme, t_minishell *data)
{
	if (type == DOUBLE_Q)
	{
		*str = ft_strjoin_alt_gc(*str, "\"", FREE_PARAM1, data);
		check_malloc(*str, data);
		*str = ft_strjoin_alt_gc(*str, lexeme, FREE_PARAM1, data);
		check_malloc(*str, data);
		*str = ft_strjoin_alt_gc(*str, "\"", FREE_PARAM1, data);
		check_malloc(*str, data);
	}
	else if (type == SINGLE_Q)
	{
		*str = ft_strjoin_alt_gc(*str, "\'", FREE_PARAM1, data);
		check_malloc(*str, data);
		*str = ft_strjoin_alt_gc(*str, lexeme, FREE_PARAM1, data);
		check_malloc(*str, data);
		*str = ft_strjoin_alt_gc(*str, "\'", FREE_PARAM1, data);
		check_malloc(*str, data);
	}
	else
	{
		*str = ft_strjoin_alt_gc(*str, lexeme, FREE_PARAM1, data);
		check_malloc(*str, data);
	}
}

static void append_space_to_string(t_lst *tokens, char **str, t_minishell *data)
{
	if ((tokens->type == WORD
			|| tokens->type == SINGLE_Q || tokens->type == DOUBLE_Q)
			&& tokens->metacharacter_after == true)
	{
		*str = ft_strjoin_alt_gc(*str, " ", FREE_PARAM1, data);
		check_malloc(*str, data);
	}
}

char	*tokens_to_str(t_lst *tokens, t_minishell *data)
{
	char	*str;

	str = NULL;
	while (tokens->type != LAST_PAR_CLOSE || tokens->next != NULL)
	{
		if (str == NULL)
		{
			str = ft_strdup(tokens->lexeme);
			check_malloc(str, data);
		}
		else
			append_lexeme_to_string(tokens->type, &str, tokens->lexeme, data);
		append_space_to_string(tokens, &str, data);
		tokens = tokens->next;
	}
	print_debug_str(str, 12,
		"\ndisplay parenthesis_string just after creating it\n");
	return (str);
}
