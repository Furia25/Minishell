/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/20 20:41:07 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_number(t_list *tokens)
{
	size_t	i;

	i = 0;
	while (tokens)
	{
		if (ft_strcmp((char *)tokens->content, "|") == 0 
			|| ft_strcmp((char *)tokens->content, "||") == 0 
			|| ft_strcmp((char *)tokens->content, "&&") == 0
			|| ft_strcmp((char *)tokens->content, "<") == 0
			|| ft_strcmp((char *)tokens->content, ">") == 0)
			i++;
		tokens = tokens->next;
	}
	return (i + 1);
}

void	fill_tab(t_list *tokens, t_leaf *command_tab)
{
	t_list *temp;

	temp = tokens;
	while (temp)
	{
		if (ft_strcmp((char *)temp->content, "|") == 0)
			command_tab->ope_after = PIPE;
		else if (ft_strcmp((char *)temp->content, "||") == 0)
			command_tab->ope_after = OR;
		else if (ft_strcmp((char *)temp->content, "&&") == 0)
			command_tab->ope_after = AND;
		else if (ft_strcmp((char *)temp->content, "<") == 0)
			command_tab->ope_after = RED_I;
		else if (ft_strcmp((char *)temp->content, ">") == 0)
			command_tab->ope_after = RED_O;
		else
		{
			temp = temp->next;
			continue ;
		}
		temp->content = NULL;
		command_tab->tokens = tokens;
		//fonctions pour command_tab->char fd_input, fd_output et returned_value;
		fill_tab(temp->next, command_tab + 1);
		return ;
	}
	command_tab->tokens = tokens;
	command_tab->ope_after = LINE_CHANGE;
}

t_leaf *create_cmd_tab(t_list *tokens)
{
	t_leaf *command_tab;

	
	command_tab = malloc(sizeof(t_leaf) * command_number(tokens));
	fill_tab(tokens, command_tab);
	return (command_tab);
}

// int	main(void)
// {
// 	char *input = "	echo	< abcde | echo	fgh	||	'ffezz'	";
// 	t_list	*tokens;
// 	t_leaf *command_tab;


// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	command_tab = create_cmd_tab(tokens);
// 	while (command_tab->ope_after != LINE_CHANGE)
// 	{
// 		while (command_tab->tokens->content)
// 		{
// 			printf("%s\n", (char*)command_tab->tokens->content);
// 			command_tab->tokens = command_tab->tokens->next;
// 		}
// 		printf("%d\n\n", command_tab->ope_after);
// 		command_tab++;
// 	}
// 	while (command_tab->tokens)
// 	{
// 		printf("%s\n", (char*)command_tab->tokens->content);
// 		command_tab->tokens = command_tab->tokens->next;
// 	}
// 	printf("%d\n", command_tab->ope_after);
	
// }
