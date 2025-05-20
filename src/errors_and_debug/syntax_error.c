/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:19:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 14:43:56 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	syntax_errors(char *str, t_minishell *data)
{
	ft_printf_fd(2, "minishell: syntax error \
near unexpected token `%s\'\n", str);
	data->exit_code = 2;
}

static bool	have_syntax_errors_op(t_lst *tokens, t_minishell *data)
{
	if (tokens->type == AND || tokens->type == PIPE
		|| tokens->type == OR)
	{
		if (tokens->next == NULL)
		{
			syntax_errors("newline", data);
			return (true);
		}
		if (tokens->next->type == AND
			|| tokens->next->type == PIPE
			|| tokens->next->type == OR)
		{
			syntax_errors(tokens->next->lexeme, data);
			return (true);
		}
	}
	return (false);
}

static bool	have_syntax_errors_redis(t_lst *tokens, t_minishell *data)
{
	if (tokens->type == RED_IN || tokens->type == RED_OUT
		|| tokens->type == RED_OUT_A || tokens->type == RED_IN_OUT
		|| tokens->type == HERE_DOC)
	{
		if (tokens->next == NULL)
		{
			syntax_errors("newline", data);
			return (true);
		}
		if (tokens->next->type != SINGLE_Q
			&& tokens->next->type != DOUBLE_Q
			&& tokens->next->type != WORD)
		{
			syntax_errors(tokens->next->lexeme, data);
			return (true);
		}
	}
	return (false);
}

static bool	have_syntax_errors_parenthesis(t_lst **tokens, t_minishell *data)
{
	t_lst	*parenth_buff;

	parenth_buff = NULL;
	if ((*tokens)->type == PAR_CLOSE)
	{
		not_interpret_chara(')', "\' (unclosed parenthesis)", data);
		return (true);
	}
	if ((*tokens)->type == PAR_OPEN)
	{
		while (*tokens)
		{
			if ((*tokens)->type == PAR_CLOSE)
				parenth_buff = *tokens;
			*tokens = (*tokens)->next;
		}
		if (parenth_buff == NULL)
		{
			not_interpret_chara('(', "\' (unclosed parenthesis)", data);
			return (true);
		}
		*tokens = parenth_buff;
	}
	return (false);
}

bool	have_syntax_errors(t_lst *tokens, t_minishell *data)
{
	if (tokens->type == AND || tokens->type == PIPE
		|| tokens->type == OR)
	{
		syntax_errors(tokens->lexeme, data);
		return (true);
	}
	while (tokens)
	{
		if (have_syntax_errors_parenthesis(&tokens, data) == true)
			return (true);
		if (have_syntax_errors_op(tokens, data) == true)
			return (true);
		if (have_syntax_errors_redis(tokens, data) == true)
			return (true);
		tokens = tokens->next;
	}
	return (false);
}
