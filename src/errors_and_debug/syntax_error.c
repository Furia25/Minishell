/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:19:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/01 23:21:51 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_error(char *str, t_minishell *data)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("\'", 2);
	//free data->gc
	//exit_minishell(data);
	data->exit_code = 2;
	return (EXIT_FAILURE);
}

int	check_syntax_errors(t_lst *tokens, t_minishell *data)
{
	t_lst	*parenth_buff;

	parenth_buff = NULL;
	if (tokens->type == AND || tokens->type == PIPE
		|| tokens->type == OR)
		return (print_error(tokens->lexeme, data));
	while (tokens)
	{
		if (tokens->type == PAR_CLOSE)
			return (not_interpret_chara(')', "\' (unclosed parenthesis)", data));
		if (tokens->type == PAR_OPEN)
		{
			while (tokens)
			{
				if (tokens->type == PAR_CLOSE)
					parenth_buff = tokens;
				tokens = tokens->next;
			}
			if (parenth_buff == NULL)
				return (not_interpret_chara('(', "\' (unclosed parenthesis)", data));
			tokens = parenth_buff;
		}
		if (tokens->type == AND || tokens->type == PIPE
			|| tokens->type == OR)
		{
			if (tokens->next == NULL)
				return (print_error("newline", data));
			if (tokens->next->type == AND
				|| tokens->next->type == PIPE
				|| tokens->next->type == OR)
				return (print_error(tokens->next->lexeme, data));
		}
		if (tokens->type == RED_IN || tokens->type == RED_OUT
			|| tokens->type == RED_OUT_A || tokens->type == RED_IN_OUT
			|| tokens->type == HERE_DOC)
		{
			if (tokens->next == NULL)
				return (print_error("newline", data));
			if (tokens->next->type != SINGLE_Q
				&& tokens->next->type != DOUBLE_Q
				&&  tokens->next->type != WORD)
				return (print_error(tokens->next->lexeme, data));
		}
		tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}
