/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:19:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/25 02:56:26 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error(char *str, t_minishell *data)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("\'", 2);
	//free data->gc
	data->exit_code = 2;
}

void	check_syntax_errors(t_lst *tokens, t_minishell *data)
{
	if (tokens->type == AND || tokens->type == PIPE
		|| tokens->type == OR)
		print_error(tokens->lexeme, data);
	while (tokens)
	{
		if (tokens->type == AND || tokens->type == PIPE
			|| tokens->type == OR)
		{
			if (tokens->next == NULL)
				print_error("newline", data);
			if (tokens->next->type == AND
				|| tokens->next->type == PIPE
				|| tokens->next->type == OR)
				print_error(tokens->next->lexeme, data);
		}
		if (tokens->type == RED_IN || tokens->type == RED_OUT
			|| tokens->type == RED_OUT_A || tokens->type == RED_IN_OUT
			|| tokens->type == HERE_DOC)
		{
			if (tokens->next == NULL)
				print_error("newline", data);
			if (tokens->next->type != SINGLE_Q
				&& tokens->next->type != DOUBLE_Q
				&&  tokens->next->type != WORD)
				print_error(tokens->next->lexeme, data);
		}
		tokens = tokens->next;
	}
}
