/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/04 23:16:30 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/incs/get_next_line.h"

size_t	cmds_number(t_list *tokens)
{
	size_t	i;
	t_list	*para_buff;

	i = 0;
	while (tokens)
	{
		if (ft_strcmp((char *)tokens->content, "(") == 0)
		{
			while (tokens)
			{
				if (ft_strcmp((char *)tokens->content, ")") == 0)
					para_buff = tokens;
				tokens = tokens->next;
			}
			tokens = para_buff;
		}
		if (ft_strcmp((char *)tokens->content, "|") == 0
			|| ft_strcmp((char *)tokens->content, "||") == 0
			|| ft_strcmp((char *)tokens->content, "&&") == 0)
			i++;
		tokens = tokens->next;
	}
	return (i + 1);
}

t_list	*parenthesis_cmd(t_leaf *command_tab, t_list *tokens, t_list **prev)
{
	t_list	*parenth_buff;
	
	command_tab->parenthesis = ON;
	while (tokens->next)
	{
		if (ft_strcmp((char *)tokens->next->content, ")") == 0)
		{
			parenth_buff = tokens->next;
			*prev = tokens;
		}
		tokens = tokens->next;
	}
	tokens = parenth_buff->next;
	ft_lstdelone(parenth_buff, free);
	if (tokens == NULL)
	{
		(*prev)->next = NULL;
		return (NULL);
	}
	return (tokens);
}

int 	check_op_after(t_leaf *command_tab, t_list **temp, t_list **prev)
{
	if (ft_strcmp((char *)(*temp)->content, "|") == 0)
		command_tab->ope_after = PIPE;
	else if (ft_strcmp((char *)(*temp)->content, "||") == 0)
		command_tab->ope_after = OR;
	else if (ft_strcmp((char *)(*temp)->content, "&&") == 0)
		command_tab->ope_after = AND;
	else
	{
		*prev = *temp;
		*temp = (*temp)->next;
		return (1);
	}
	return (0);
}

void	fill_tab(t_leaf *command_tab, t_list *tokens)
{
	t_list	*temp;
	t_list	*parenth_buff;
	t_list	*prev;

	temp = tokens;
	while (temp)
	{
		if (ft_strcmp((char *)temp->content, "(") == 0)
		{
			tokens = temp->next;
			ft_lstdelone(temp, free);
			temp = parenthesis_cmd(command_tab, tokens, &prev);
			if (temp == NULL)
				continue ;
		}
		if (check_op_after(command_tab, &temp, &prev) == 1)
			continue ;
		fill_tab(command_tab + 1, temp->next);
		command_tab->tokens = tokens;
		prev->next = NULL;
		ft_lstdelone(temp, free);
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
		command_tab->parenthesis = OFF;
		command_tab->handle_here_doc = ON;
		command_tab++;
		i++;
	}
}

t_leaf *create_cmd_tab(t_list *tokens)
{
	t_leaf *command_tab;
	size_t	commands_number;

	commands_number = cmds_number(tokens);
	command_tab = malloc(sizeof(t_leaf) * commands_number);
	initialise_cmds_fd(command_tab, commands_number);
	fill_tab(command_tab, tokens);
	return (command_tab);
}

// int	main(void)
// {
// 	char *input = "cat && (< Makefile cat)";
// 	t_list	*tokens;
// 	t_leaf *command_tab;


// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	command_tab = create_cmd_tab(tokens);
// 	while (command_tab->ope_after != LINE_CHANGE)
// 	{
// 		printf("new cmd : \n\n");
// 		while (command_tab->tokens)
// 		{
// 			printf("token : %s\n", (char*)command_tab->tokens->content);
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
// 		printf("token : %s\n", (char*)command_tab->tokens->content);
// 		command_tab->tokens = command_tab->tokens->next;
// 	}
// 	printf("fd_in : %d\n", command_tab->fd_input);
// 	printf("fd_out : %d\n", command_tab->fd_output);
// 	printf("parenthesis : %d\n", command_tab->parenthesis);
// 	printf("ope_after : %d\n\n", command_tab->ope_after);

// }
