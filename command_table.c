/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:43:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/25 05:08:42 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/incs/get_next_line.h"

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

void	handle_red_input(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon erreur
	if (command_tab->fd_input != 0)
		close(command_tab->fd_input);
	command_tab->fd_input = open((char*)tokens->next->content, O_RDONLY);//truncate + droit
}

void	handle_red_output(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);
	command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

void	handle_red_output_append(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);//secur close
	command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

void	handle_hear_doc(t_leaf *command_tab, t_list *tokens)
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
	if (command_tab->fd_input != 0)
		close(command_tab->fd_input);
	command_tab->fd_input = open("/tmp/here_doc", O_RDONLY);
	unlink("/tmp/here_doc");
}

t_list *	handle_redis_before_cmd(t_leaf *command_tab, t_list *tokens)
{
	while (ft_strcmp((char *)tokens->content, "<") == 0
		|| ft_strcmp((char *)tokens->content, ">") == 0
		|| ft_strcmp((char *)tokens->content, ">>") == 0
		|| ft_strcmp((char *)tokens->content, "<<") == 0)
	{
		if (ft_strcmp((char *)tokens->content, "<") == 0)
			handle_red_input(command_tab, tokens);
		if (ft_strcmp((char *)tokens->content, ">") == 0)
			handle_red_output(command_tab, tokens);
		if (ft_strcmp((char *)tokens->content, "<<") == 0)
			handle_hear_doc(command_tab, tokens);
		if (ft_strcmp((char *)tokens->content, ">>") == 0)
			handle_red_output_append(command_tab, tokens);
		tokens = tokens->next->next;
	}
	return (tokens);
}

t_list *	handle_redis_after_cmd(t_leaf *command_tab, t_list *tokens)
{
	if (ft_strcmp((char *)tokens->content, "<") == 0)
	{
		handle_red_input(command_tab, tokens);
		free(tokens->content);
		tokens->content = NULL;
		return (tokens->next->next);
	}
	if (ft_strcmp((char *)tokens->content, ">") == 0)
	{
		handle_red_output(command_tab, tokens);
		free(tokens->content);
		tokens->content = NULL;
		return (tokens->next->next);
	}
	if (ft_strcmp((char *)tokens->content, "<<") == 0)
	{
		handle_hear_doc(command_tab, tokens);
		free(tokens->content);
		tokens->content = NULL;
		return (tokens->next->next);
	}
	handle_red_output_append(command_tab, tokens);
	free(tokens->content);
	tokens->content = NULL;
	return (tokens->next->next);
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
			|| ft_strcmp((char *)temp->content, ">") == 0
			|| ft_strcmp((char *)temp->content, "<<") == 0
			|| ft_strcmp((char *)temp->content, ">>") == 0)
		{
			temp = handle_redis_after_cmd(command_tab, temp);
			continue ;
		}
		else
		{
			temp = temp->next;
			continue ;
		}
		free(temp->content);
		temp->content = NULL;
		command_tab->tokens = tokens;
		//fonctions pour command_tab->char fd_input, fd_output et returned_value;
		fill_tab(command_tab + 1, temp->next);
		return ;
	}
	free(temp->content);
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

int	main(void)
{
	char *input = "cat <<eof| echo pipi > caca";
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
