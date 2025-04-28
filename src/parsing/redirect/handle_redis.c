/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:56:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 05:24:32 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	handle_red_input(t_leaf *command_tab, char *file, t_minishell *data);
void	handle_red_output(t_leaf *command_tab, char *file, t_minishell *data);
void	handle_red_output_append(t_leaf *command_tab, char *file, t_minishell *data);
void	handle_red_input_output(t_leaf *command_tab, char *file, t_minishell *data);

static bool	check_redi(t_leaf *command_tab, t_lst *token, t_minishell *data)
{
	if (token->type == RED_IN || token->type == RED_OUT
		|| token->type == RED_OUT_A || token->type == RED_IN_OUT)
	{
		if (token->type == RED_IN)
			handle_red_input(command_tab, token->next->lexeme, data);
		if (token->type == RED_OUT)
			handle_red_output(command_tab, token->next->lexeme, data);
		if (token->type == RED_OUT_A)
			handle_red_output_append(command_tab, token->next->lexeme, data);
		if (token->type == RED_IN_OUT)
			handle_red_input_output(command_tab, token->next->lexeme, data);
		return (true);
	}
	return (false);
}

static void	del_reds_tokens(t_lst *token)
{
	lstdelone(token->next, free);
	lstdelone(token, free);
}

void	handle_reds_and_del(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;
	t_lst	*prev;

	while (command_tab->tokens != NULL
		&& check_redi(command_tab, command_tab->tokens, data) == true)
	{
		temp = command_tab->tokens;
		command_tab->tokens = command_tab->tokens->next->next;
		del_reds_tokens(temp);
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (check_redi(command_tab, temp, data) == true)
		{
			prev->next = temp->next->next;
			del_reds_tokens(temp);
			temp = prev->next;
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}