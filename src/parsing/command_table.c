/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 04:10:53 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	cmds_number(t_lst *tokens)
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

t_lst	*parenthesis_cmd(t_leaf *command_tab, t_lst *tokens, t_lst **prev)
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
	lstdelone(parenth_buff, free);
	if (tokens == NULL)
	{
		(*prev)->next = NULL;
		return (NULL);
	}
	return (tokens);
}

int	check_op_after(t_leaf *command_tab, t_lst **temp, t_lst **prev)
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
		return (1);
	}
	return (0);
}

void	fill_tab(t_leaf *command_tab, t_lst *tokens)
{
	t_lst	*temp;
	t_lst	*prev;

	temp = tokens;
	while (temp)
	{
		if (temp->type == PAR_OPEN)
		{
			tokens = temp->next;
			lstdelone(temp, free);
			temp = parenthesis_cmd(command_tab, tokens, &prev);
			if (temp == NULL)
				continue ;
		}
		if (check_op_after(command_tab, &temp, &prev) == 1)
			continue ;
		fill_tab(command_tab + 1, temp->next);
		command_tab->tokens = tokens;
		prev->next = NULL;
		lstdelone(temp, free);
		return ;
	}
	command_tab->tokens = tokens;
	command_tab->ope_after = LINE_CHANGE;
}

void	initialise_cmds_fd(t_leaf *command_tab, size_t	commands_number)
{
	size_t	i;

	i = 0;
	while (i < commands_number)
	{
		command_tab->fd_input = 0;
		command_tab->fd_output = 1;
		command_tab->parenthesis = false;
		command_tab++;
		i++;
	}
}

t_leaf	*create_cmd_tab(t_lst *tokens, t_minishell *data)
{
	t_leaf *command_tab;
	size_t	commands_number;

	commands_number = cmds_number(tokens);
	command_tab = malloc(sizeof(t_leaf) * commands_number);
	check_malloc(command_tab, data);
	initialise_cmds_fd(command_tab, commands_number);
	fill_tab(command_tab, tokens);
	return (command_tab);
}

// int	main(void)
// {
// 	char *input = "((test";
// 	t_lst	*tokens;
// 	t_leaf *command_tab;

// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	check_syntax_errors(tokens);
// 	fusion_quote_token(tokens);
// 	command_tab = create_cmd_tab(tokens);
// 	while (command_tab->ope_after != LINE_CHANGE)
// 	{
// 		printf("new cmd : \n\n");
// 		while (command_tab->tokens)
// 		{
// 			printf("token : %s\n", command_tab->tokens->lexeme);
// 			command_tab->tokens = command_tab->tokens->next;
// 		}
// 		printf("fd_in : %d\n", command_tab->fd_input);
// 		printf("fd_out : %d\n", command_tab->fd_output);
// 		printf("parenthesis : %d\n", command_tab->parenthesis);
// 		printf("ope_after : %d\n\n", command_tab->ope_after);
// 		command_tab++;
// 	}
// 	printf("new cmd : \n\n");
// 	while (command_tab->tokens)
// 	{
// 		printf("token : %s\n", command_tab->tokens->lexeme);
// 		command_tab->tokens = command_tab->tokens->next;
// 	}
// 	printf("fd_in : %d\n", command_tab->fd_input);
// 	printf("fd_out : %d\n", command_tab->fd_output);
// 	printf("parenthesis : %d\n", command_tab->parenthesis);
// 	printf("ope_after : %d\n\n", command_tab->ope_after);
// }
