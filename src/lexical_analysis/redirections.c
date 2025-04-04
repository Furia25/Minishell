/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 23:39:47 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/04 23:25:26 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

void	handle_red_input(t_leaf *command_tab, char *file)
{
	int	fd_buff;

	//verifier que le fichier existe bien sinon erreur
	if (command_tab->fd_input == -1 || command_tab->fd_output == -1)
		return ;
	if (command_tab->fd_input != 0)
		close(command_tab->fd_input);
	command_tab->fd_input = open(file, O_RDONLY);//truncate + droit
	if (command_tab->fd_input == -1)
		perror(file);
	if (ft_strncmp("/tmp/here_doc", file, 13) == 0)
		unlink(file);
}

void	handle_red_output(t_leaf *command_tab, char *file)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_input == -1 || command_tab->fd_output == -1)
		return ;
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);
	command_tab->fd_output = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (command_tab->fd_output == -1)
		perror(file);
}

void	handle_red_output_append(t_leaf *command_tab, char *file)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_input == -1 || command_tab->fd_output == -1)
		return ;
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);//secur close
	command_tab->fd_output = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (command_tab->fd_output == -1)
		perror(file);
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

void	write_in_here_doc_file(char *eof, int fd)
{
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(input, eof) == 0)
		{
			free(input);
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
}

char	*handle_here_doc(t_leaf *command_tab, char *eof)
{
	char	*input;
	char	*here_doc_file;
	int	i;
	int	fd;

	if (command_tab->fd_input == -1)
		return (NULL);
	here_doc_file = ft_strdup("/tmp/here_doc");
	fd = open_new_here_doc_file(command_tab, &here_doc_file);
	write_in_here_doc_file(eof, fd);
	close(fd);
	return (here_doc_file);
}

int	check_redi(t_leaf *command_tab, t_list *token)
{
	if (ft_strcmp((char *)token->content, "<") == 0
		|| ft_strcmp((char *)token->content, ">") == 0
		|| ft_strcmp((char *)token->content, ">>") == 0)
	{
		if (ft_strcmp((char *)token->content, "<") == 0)
			handle_red_input(command_tab, (char*)token->next->content);
		if (ft_strcmp((char *)token->content, ">") == 0)
			handle_red_output(command_tab, (char*)token->next->content);
		if (ft_strcmp((char *)token->content, ">>") == 0)
			handle_red_output_append(command_tab, (char*)token->next->content);
		return (0);
	}
	return (1);
}

void	del_reds_tokens(t_list *token)
{
	ft_lstdelone(token->next, free);
	ft_lstdelone(token, free);
}

void	handle_reds_and_del(t_leaf *command_tab)
{
	t_list	*temp;
	t_list	*prev;

	while (command_tab->tokens != NULL 
		&& check_redi(command_tab, command_tab->tokens) == 0)
	{
		temp = command_tab->tokens;
		command_tab->tokens = command_tab->tokens->next->next;
		del_reds_tokens(temp);
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (check_redi(command_tab, temp) == 0)
		{
			prev->next = temp->next->next;
			del_reds_tokens(temp);
			temp = prev->next;
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}

void	here_doc_to_red_input(t_leaf *command_tab)
{
	t_list	*temp;
	char	*here_doc_file;

	temp = command_tab->tokens;
	while (temp)
	{
		if (ft_strcmp((char *)temp->content, "<<") == 0)
		{
			here_doc_file = handle_here_doc(command_tab, (char*)temp->next->content);
			free(temp->content);
			free(temp->next->content);
			temp->content = ft_substr("<", 0, 1);
			temp->next->content = here_doc_file;
			temp = temp->next->next;
		}
		else
			temp = temp->next;
	}

}

void	handle_all_here_doc(t_leaf *command_tab)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		here_doc_to_red_input(command_tab);
		command_tab++;
	}
	here_doc_to_red_input(command_tab);
}

void	rm_here_doc_files(t_leaf *command_tab)
{
	t_list	*temp;

	while (command_tab->ope_after != LINE_CHANGE)
	{
		temp = command_tab->tokens;
		while (temp)
		{
			if (ft_strcmp((char *)temp->content, "<") == 0 &&
				ft_strncmp("/tmp/here_doc", (char *)temp->next->content, 13) == 0)
				unlink((char *)temp->next->content);
			temp = temp->next;
		}
		command_tab++;
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (ft_strcmp((char *)temp->content, "<") == 0 &&
			ft_strncmp("/tmp/here_doc", (char *)temp->next->content, 13) == 0)
			unlink((char *)temp->next->content);
		temp = temp->next;
	}
	
}

// int	main(void)
// {
// 	char *input = "<<eof cat >caca || <<eof <ww cat >pipi";
// 	t_list	*tokens;
// 	t_leaf *command_tab;


// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	command_tab = create_cmd_tab(tokens);
// 	handle_all_here_doc(command_tab);
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



