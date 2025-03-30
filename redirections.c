/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 23:39:47 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/30 06:37:35 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

void	handle_red_input(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon erreur
	if (command_tab->fd_input == -1)
		return ;
	if (command_tab->fd_input != 0)
		close(command_tab->fd_input);
	command_tab->fd_input = open((char*)tokens->next->content, O_RDONLY);//truncate + droit
	if (command_tab->fd_input == -1)
		perror((char*)tokens->next->content);
}

void	handle_red_output(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_output == -1)
		return ;
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);
	command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (command_tab->fd_output == -1)
		perror((char*)tokens->next->content);
}

void	handle_red_output_append(t_leaf *command_tab, t_list *tokens)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_output == -1)
		return ;
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);//secur close
	command_tab->fd_output = open((char*)tokens->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (command_tab->fd_output == -1)
		perror((char*)tokens->next->content);
}

int	open_new_here_doc_file(t_leaf *command_tab, char **here_doc_file)
{
	int		fd;
	int		i;

	fd = open(*here_doc_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
	if (fd == -1 && errno == EEXIST)
	{
		i = 1;
		while (errno == EEXIST)
		{
			free(*here_doc_file);
			*here_doc_file = ft_strjoin_alt("/tmp/here_doc", ft_itoa(i));
			fd = open(*here_doc_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
			if (fd != -1)
				break;
			i++;
		}
	}
	if (fd == -1 && errno != EEXIST)
	{
		command_tab->fd_input = -1;
		perror("here doc");
		exit(1);//a secur
	}
	return (fd);
}

void	write_in_here_doc_file(t_list *tokens, int fd)
{
	char	*input;

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
}

void	handle_here_doc(t_leaf *command_tab, t_list *tokens)
{
	char	*input;
	char	*here_doc_file;
	int	i;
	int	fd;

	if (command_tab->fd_input == -1)
		return ;
	if (command_tab->fd_input != 0)
		close(command_tab->fd_input);
	here_doc_file = ft_strdup("/tmp/here_doc");
	fd = open_new_here_doc_file(command_tab, &here_doc_file);
	write_in_here_doc_file(tokens, fd);
	close(fd);
	command_tab->fd_input = open(here_doc_file, O_RDONLY);
	if (command_tab->fd_input == -1)
		perror((char*)tokens->next->content);
	unlink(here_doc_file);
	free(here_doc_file);
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
	char *input = "cmd0 &&((cmd1 <<eof  || cmd2) && cmd3 > caca) && cmd4";
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
		printf("parenthesis : %d\n", command_tab->parenthesis);
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
	printf("parenthesis : %d\n", command_tab->parenthesis);
	printf("ope_after : %d\n\n", command_tab->ope_after);

}



