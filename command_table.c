/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/23 05:51:04 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	cmds_number(t_list *tokens)
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

t_list *	handle_redis_before_cmd(t_leaf *command_tab, t_list *tokens)
{
	while (ft_strcmp((char *)tokens->content, "<") == 0
		|| ft_strcmp((char *)tokens->content, ">") == 0)
	{
		if (ft_strcmp((char *)tokens->content, "<") == 0)
		{
			//verifier que le fichier existe bien sinon erreur
			command_tab->fd_input = open((char*)tokens->next->content, O_RDONLY);//truncate + droit
		}
		if (ft_strcmp((char *)tokens->content, ">") == 0)
		{
			//verifier que le fichier existe bien sinon le creer
			command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY);
		}
		tokens = tokens->next->next;
	}
	return (tokens);
}

t_list *	handle_redis_after_cmd(t_leaf *command_tab, t_list *tokens)
{
	if (ft_strcmp((char *)tokens->content, "<") == 0)
	{
		//verifier que le fichier existe bien sinon erreur
		command_tab->fd_input = open((char*)tokens->next->content, O_RDONLY);//truncate + droit
		tokens->content = NULL;
		return (tokens->next->next);
	}
	if (ft_strcmp((char *)tokens->content, ">") == 0)
	{
		//verifier que le fichier existe bien sinon le creer
		command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY);
		tokens->content = NULL;
		return (tokens->next->next);
	}
}

void	fill_tab(t_leaf *command_tab, t_list *tokens)
{
	t_list *temp;

	tokens = handle_redis_before_cmd(command_tab, tokens);
	temp = tokens;
	while (ft_strcmp((char *)temp->content, "\n") != 0)
	{
		if (ft_strcmp((char *)temp->content, "|") == 0)
			command_tab->ope_after = PIPE;
		else if (ft_strcmp((char *)temp->content, "||") == 0)
			command_tab->ope_after = OR;
		else if (ft_strcmp((char *)temp->content, "&&") == 0)
			command_tab->ope_after = AND;
		else if (ft_strcmp((char *)temp->content, "<") == 0
			|| ft_strcmp((char *)temp->content, ">") == 0)
		{
			temp = handle_redis_after_cmd(command_tab, temp);
			continue ;
		}
		else
		{
			temp = temp->next;
			continue ;
		}
		temp->content = NULL;
		command_tab->tokens = tokens;
		//fonctions pour command_tab->char fd_input, fd_output et returned_value;
		fill_tab(command_tab + 1, temp->next);
		return ;
	}
	temp->content = NULL;
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
// 	char *input = "<a >c echo abcde >caca | 'vv' > caca";
// 	t_list	*tokens;
// 	t_leaf *command_tab;


// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	command_tab = create_cmd_tab(tokens);
// 	while (command_tab->ope_after != LINE_CHANGE)
// 	{
// 		printf("new cmd : \n\n");
// 		while (command_tab->tokens->content)
// 		{
// 			printf("token : %s\n", (char*)command_tab->tokens->content);
// 			command_tab->tokens = command_tab->tokens->next;
// 		}
// 		printf("fd_in : %d\n", command_tab->fd_input);
// 		printf("fd_out : %d\n", command_tab->fd_output);
// 		printf("ope_after : %d\n\n", command_tab->ope_after);
// 		command_tab++;
// 	}
// 	printf("new cmd : \n\n");
// 	while (command_tab->tokens->content)
// 	{
// 		printf("token : %s\n", (char*)command_tab->tokens->content);
// 		command_tab->tokens = command_tab->tokens->next;
// 	}
// 	printf("fd_in : %d\n", command_tab->fd_input);
// 	printf("fd_out : %d\n", command_tab->fd_output);
// 	printf("ope_after : %d\n\n", command_tab->ope_after);

// }
