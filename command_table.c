/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/23 03:27:52 by alpayet          ###   ########.fr       */
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

void	fill_tab(t_leaf *command_tab, t_list *tokens)
{
	t_list *temp;

	temp = tokens;
	while (ft_strcmp((char *)temp->content, "\n") != 0)
	{
		if (ft_strcmp((char *)temp->content, "|") == 0)
			command_tab->ope_after = PIPE;
		else if (ft_strcmp((char *)temp->content, "||") == 0)
			command_tab->ope_after = OR;
		else if (ft_strcmp((char *)temp->content, "&&") == 0)
			command_tab->ope_after = AND;
		else if (ft_strcmp((char *)temp->content, "<") == 0)
		{
			//verifier que le fichier existe bien sinon erreur
			command_tab->fd_input = open((char*)temp->next->content, O_RDONLY);//truncate + droit
			fill_tab(command_tab, temp->next->next);
		}
		else if (ft_strcmp((char *)temp->content, ">") == 0)
		{
			//verifier que le fichier existe bien sinon le creer
			command_tab->fd_output = open((char*)temp->next->content, O_WRONLY);
			fill_tab(command_tab, temp->next->next);
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

void	initialise_cmds_fd(t_leaf *command_tab)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		command_tab->fd_input = 0;
		command_tab->fd_output = 1;
		command_tab++;
	}
	command_tab->fd_input = 0;
	command_tab->fd_output = 1;
}

t_leaf *create_cmd_tab(t_list *tokens)
{
	t_leaf *command_tab;


	command_tab = malloc(sizeof(t_leaf) * command_number(tokens));
	fill_tab(command_tab, tokens);
	initialise_cmds_fd(command_tab);
	return (command_tab);
}

int	main(void)
{
	char *input = "echo abcde | 'vv' >fdds";
	t_list	*tokens;
	t_leaf *command_tab;


	tokens = NULL;
	create_tokens(&tokens, input);
	command_tab = create_cmd_tab(tokens);
	while (command_tab->ope_after != LINE_CHANGE)
	{
		printf("new cmd : \n\n");
		while (command_tab->tokens->content)
		{
			printf("token : %s\n", (char*)command_tab->tokens->content);
			command_tab->tokens = command_tab->tokens->next;
		}
		printf("fd_in : %d\n", command_tab->fd_input);
		printf("fd_out : %d\n", command_tab->fd_output);
		printf("ope_after : %d\n\n", command_tab->ope_after);
		command_tab++;
	}
	printf("new cmd : \n\n");
	while (command_tab->tokens->content)
	{
		printf("token : %s\n", (char*)command_tab->tokens->content);
		command_tab->tokens = command_tab->tokens->next;
	}
	printf("fd_in : %d\n", command_tab->fd_input);
	printf("fd_out : %d\n", command_tab->fd_output);
	printf("ope_after : %d\n\n", command_tab->ope_after);

}
