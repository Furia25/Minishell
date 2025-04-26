/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 23:39:47 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 02:27:06 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
char	*handle_subshell_in_lexeme(char *str, t_minishell *data);

void	handle_red_input(t_leaf *command_tab, char *file, t_minishell *data)
{
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

void	handle_red_output(t_leaf *command_tab, char *file, t_minishell *data)
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

void	handle_red_output_append(t_leaf *command_tab, char *file, t_minishell *data)
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

void	handle_red_input_output(t_leaf *command_tab, char *file, t_minishell *data)
{
	//verifier que le fichier existe bien sinon le creer
	if (command_tab->fd_input == -1 || command_tab->fd_output == -1)
		return ;
	if (command_tab->fd_input != 1)
		close(command_tab->fd_input);//secur close
	command_tab->fd_input = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (command_tab->fd_input == -1)
		perror(file);
}

int	open_new_here_doc_file(t_leaf *command_tab, char **here_doc_file, t_minishell *data)
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
			*here_doc_file = ft_strjoin_alt("/tmp/here_doc", ft_itoa(i), FREE_PARAM2);
			check_malloc(*here_doc_file, data);
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

void	write_in_here_doc_file(t_lst *token_eof, int fd, t_minishell *data)
{
	char	*input;
	char	*buff;

	while (1)
	{
		input = readline("> ");
		check_malloc(input, data);
		if (ft_strcmp(input, token_eof->lexeme) == 0)
		{
			free(input);
			break ;
		}
		if (token_eof->type != SINGLE_Q)
		{
			buff = handle_subshell_in_lexeme(input, data);
			free(input);
			input = buff;
		}
		if (DEBUG == 4 || DEBUG == 1)
			ft_printf("input: %s\n", input);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
}

char	*handle_here_doc(t_leaf *command_tab, t_lst	*token_eof, t_minishell *data)
{
	char	*here_doc_file;
	int	fd;

	if (command_tab->fd_input == -1)
		return (NULL);
	here_doc_file = ft_strdup("/tmp/here_doc");
	check_malloc(here_doc_file, data);
	fd = open_new_here_doc_file(command_tab, &here_doc_file, data);
	write_in_here_doc_file(token_eof, fd, data);
	close(fd);
	return (here_doc_file);
}

int	check_redi(t_leaf *command_tab, t_lst *token, t_minishell *data)
{
	if (token->type == RED_IN || token->type == RED_OUT
		|| token->type == RED_OUT_A || token->type == RED_IN_OUT)
	{
		if (token->type == RED_IN)
			handle_red_input(command_tab, token->next->lexeme, data);
		if (token->type == RED_OUT)
			handle_red_output(command_tab, token->next->lexeme, data);
		if (token->type == RED_OUT_A)
			handle_red_output_append(command_tab, token->next->lexeme, data);
		if (token->type == RED_IN_OUT)
			handle_red_input_output(command_tab, token->next->lexeme, data);
		return (0);
	}
	return (1);
}

void	del_reds_tokens(t_lst *token)
{
	lstdelone(token->next, free);
	lstdelone(token, free);
}

void	handle_reds_and_del(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;
	t_lst	*prev;

	while (command_tab->tokens != NULL
		&& check_redi(command_tab, command_tab->tokens, data) == 0)
	{
		temp = command_tab->tokens;
		command_tab->tokens = command_tab->tokens->next->next;
		del_reds_tokens(temp);
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (check_redi(command_tab, temp, data) == 0)
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

void	here_doc_to_red_input(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;
	char	*here_doc_file;

	temp = command_tab->tokens;
	while (temp)
	{
		if (temp->type == HERE_DOC)
		{
			here_doc_file = handle_here_doc(command_tab, temp->next, data);
			free(temp->next->lexeme);
			temp->type = RED_IN;
			temp->next->lexeme = here_doc_file;
			temp = temp->next->next;
		}
		else
			temp = temp->next;
	}
}

void	handle_all_here_doc(t_leaf *command_tab, t_minishell *data)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		here_doc_to_red_input(command_tab, data);
		command_tab++;
	}
	here_doc_to_red_input(command_tab, data);
}

void	rm_here_doc_files(t_leaf *command_tab)
{
	t_lst	*temp;

	while (command_tab->ope_after != LINE_CHANGE)
	{
		temp = command_tab->tokens;
		while (temp)
		{
			if (temp->type == RED_IN &&
				ft_strncmp("/tmp/here_doc", temp->next->lexeme, 13) == 0)
				unlink(temp->next->lexeme);
			temp = temp->next;
		}
		command_tab++;
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (temp->type == RED_IN &&
			ft_strncmp("/tmp/here_doc", temp->next->lexeme, 13) == 0)
			unlink(temp->next->lexeme);
		temp = temp->next;
	}
}

// int	main(void)
// {
// 	char *input = "<<eof <<eof cat >pipi";
// 	t_lst	*tokens;
// 	t_leaf *command_tab;


// 	tokens = NULL;
// 	create_tokens(&tokens, input);
//	fusion_quote_token(tokens);
// 	command_tab = create_cmd_tab(tokens);
// 	handle_all_here_doc(command_tab);
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



