/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/21 01:54:14 by alpayet          ###   ########.fr       */
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

static t_lst	*node_after_parenthesis(t_lst **prev, t_lst *curr)
{
	t_lst	*parenth_buff;

	parenth_buff = NULL;
	while (curr != NULL)
	{
		if (curr->type == PAR_CLOSE)
			parenth_buff = curr;
		curr = curr->next;
	}
	*prev = parenth_buff;
	(*prev)->type = LAST_PAR_CLOSE;
	curr = parenth_buff->next;
	return (curr);
}

static bool	is_op_node(t_leaf *cmd, t_lst **prev, t_lst **curr)
{
	if ((*curr)->type == PIPE)
		cmd->ope_after = PIPE;
	else if ((*curr)->type == OR)
		cmd->ope_after = OR;
	else if ((*curr)->type == AND)
		cmd->ope_after = AND;
	else
	{
		*prev = *curr;
		*curr = (*curr)->next;
		return (false);
	}
	return (true);
}

static void	fill_tab(t_leaf *command_tab, t_lst *tokens, t_minishell *data)
{
	t_lst	*curr;
	t_lst	*prev;

	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->type == PAR_OPEN)
		{
			command_tab->parenthesis = true;
			curr = node_after_parenthesis(&prev, tokens);
			if (curr == NULL)
				continue ;
		}
		if (is_op_node(command_tab, &prev, &curr) == false)
			continue ;
		fill_tab(command_tab + 1, curr->next, data);
		command_tab->tokens = tokens;
		prev->next = NULL;
		gc_free_node(curr, data);
		return ;
	}
	command_tab->tokens = tokens;
	command_tab->ope_after = LINE_CHANGE;
}

t_leaf	*create_cmd_tab(t_lst *tokens, t_minishell *data)
{
	t_leaf	*command_tab;
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
