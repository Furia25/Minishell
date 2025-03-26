/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 23:39:47 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/26 09:29:45 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_red_input(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon erreur
	if (command_tab->fd_input == -1)
		return ;
	if (command_tab->fd_input != 0 && command_tab->fd_input != -1)
		close(command_tab->fd_input);
	command_tab->fd_input = open((char*)tokens->next->content, O_RDONLY);//truncate + droit
	if (command_tab->fd_input == -1)
		perror((char*)tokens->next->content);
}

void	handle_red_output(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_input == -1)
		return ;
	if (command_tab->fd_output != 1 && command_tab->fd_input != -1)
		close(command_tab->fd_output);
	command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (command_tab->fd_input == -1)
		perror((char*)tokens->next->content);
}

void	handle_red_output_append(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_input == -1)
		return ;
	if (command_tab->fd_output != 1 && command_tab->fd_input != -1)
		close(command_tab->fd_output);//secur close
	command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (command_tab->fd_input == -1)
		perror((char*)tokens->next->content);
}

void	handle_here_doc(t_leaf *command_tab, t_list *tokens)
{
	char	*input;
	char	*str;
	int		fd;

	fd = open("/tmp/here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(input, (char*)tokens->next->content) == 0)
		{
			free(input);
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	if (command_tab->fd_input != 0 && command_tab->fd_input != -1)
		close(command_tab->fd_input);
	command_tab->fd_input = open("/tmp/here_doc", O_RDONLY);
	if (command_tab->fd_input == -1)
		perror((char*)tokens->next->content);
	unlink("/tmp/here_doc");
}

int	consider_here_doc(t_leaf *command_tab, t_list *tokens)
{
	if (ft_strcmp((char *)tokens->content, "<<") == 0)
	{
		handle_here_doc(command_tab, tokens);
		return (0);
	}
	return (1);
}
int	consider_redis(t_leaf *command_tab, t_list *tokens)
{
	if (ft_strcmp((char *)tokens->content, "<") == 0
		|| ft_strcmp((char *)tokens->content, ">") == 0
		|| ft_strcmp((char *)tokens->content, ">>") == 0)
	{
		if (ft_strcmp((char *)tokens->content, "<") == 0)
			handle_red_input(command_tab, tokens);
		if (ft_strcmp((char *)tokens->content, ">") == 0)
			handle_red_output(command_tab, tokens);
		if (ft_strcmp((char *)tokens->content, ">>") == 0)
			handle_red_output_append(command_tab, tokens);
		return (0);
	}
	return (1);
}

void	handle_red_and_del(t_leaf *command_tab, int (*is_redi)())
{
	t_list	*temp;
	t_list	*prev;

	while (command_tab->tokens != NULL && is_redi(command_tab, command_tab->tokens) == 0)
	{
		temp = command_tab->tokens;
		command_tab->tokens = command_tab->tokens->next->next;
		ft_lstdelone(temp->next, free);
		ft_lstdelone(temp, free);
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (is_redi(command_tab, temp) == 0)
		{
			prev->next = temp->next->next;
			ft_lstdelone(temp->next, free);
			ft_lstdelone(temp, free);
			temp = prev->next;
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}

void	handle_redirections(t_leaf *command_tab)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		handle_red_and_del(command_tab, consider_here_doc);
		handle_red_and_del(command_tab, consider_redis);
		command_tab++;
	}
	handle_red_and_del(command_tab, consider_here_doc);
	handle_red_and_del(command_tab, consider_redis);
}

int	main(void)
{
	char *input = "<lo <l cat <<e | caca >pipi";
	t_list	*tokens;
	t_leaf *command_tab;


	tokens = NULL;
	create_tokens(&tokens, input);
	command_tab = create_cmd_tab(tokens);
	handle_redirections(command_tab);
	while (command_tab->ope_after != LINE_CHANGE)
	{
		printf("new cmd : \n\n");
		while (command_tab->tokens)
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
	while (command_tab->tokens)
	{
		printf("token : %s\n", (char*)command_tab->tokens->content);
		command_tab->tokens = command_tab->tokens->next;
	}
	printf("fd_in : %d\n", command_tab->fd_input);
	printf("fd_out : %d\n", command_tab->fd_output);
	printf("ope_after : %d\n\n", command_tab->ope_after);

}



