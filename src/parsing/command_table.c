/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/30 15:26:57 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	cmds_number(t_lst *tokens)
{
	size_t	i;
	t_lst	*parenth_buff;

	i = 0;
	parenth_buff = NULL;
	while (tokens)
	{
		if (tokens->type == PAR_OPEN)
		{
			while (tokens)
			{
				if (tokens->type == PAR_CLOSE)
					parenth_buff = tokens;
				tokens = tokens->next;
			}
			tokens = parenth_buff;
		}
		if (tokens->type == PIPE
			|| tokens->type == OR || tokens->type == AND)
			i++;
		tokens = tokens->next;
	}
	return (i + 1);
}

static t_lst	*parenthesis_cmd(t_leaf *command_tab, t_lst *tokens, t_lst **prev, t_minishell *data)
{
	t_lst	*parenth_buff;

	command_tab->parenthesis = true;
	while (tokens->next)
	{
		if (tokens->next->type == PAR_CLOSE)
		{
			parenth_buff = tokens->next;
			*prev = tokens;
		}
		tokens = tokens->next;
	}
	tokens = parenth_buff->next;
	gc_free_node(parenth_buff, data);
	if (tokens == NULL)
	{
		(*prev)->next = NULL;
		return (NULL);
	}
	return (tokens);
}

static bool	check_op_after(t_leaf *command_tab, t_lst **temp, t_lst **prev)
{
	if ((*temp)->type == PIPE)
		command_tab->ope_after = PIPE;
	else if ((*temp)->type == OR)
		command_tab->ope_after = OR;
	else if ((*temp)->type == AND)
		command_tab->ope_after = AND;
	else
	{
		*prev = *temp;
		*temp = (*temp)->next;
		return (false);
	}
	return (true);
}

static void	fill_tab(t_leaf *command_tab, t_lst *tokens, t_minishell *data)
{
	t_lst	*temp;
	t_lst	*prev;

	temp = tokens;
	while (temp)
	{
		if (temp->type == PAR_OPEN)
		{
			tokens = temp->next;
			gc_free_node(temp, data);
			temp = parenthesis_cmd(command_tab, tokens, &prev);
			if (temp == NULL)
				continue ;
		}
		if (check_op_after(command_tab, &temp, &prev) == false)
			continue ;
		fill_tab(command_tab + 1, temp->next);
		command_tab->tokens = tokens;
		prev->next = NULL;
		gc_free_node(temp, data);
		return ;
	}
	command_tab->tokens = tokens;
	command_tab->ope_after = LINE_CHANGE;
}

t_leaf	*create_cmd_tab(t_lst *tokens, t_minishell *data)
{
	t_leaf *command_tab;
	size_t	i;
	size_t	commands_number;

	commands_number = cmds_number(tokens);
	command_tab = malloc(sizeof(t_leaf) * commands_number);
	check_malloc(command_tab, data);
	i = 0;
	while (i < commands_number)
	{
		command_tab[i].fd_input = STDIN_FILENO;
		command_tab[i].fd_output = STDOUT_FILENO;
		command_tab[i].parenthesis = false;
		i++;
	}
	fill_tab(command_tab, tokens, data);
	return (command_tab);
}
