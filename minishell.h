/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/03 15:06:56 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/incs/libft.h"
# include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef DEBUG
#  define DEBUG 1
# endif

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_OPERATOR_CONTROL
}	t_node_type;

typedef enum s_operator
{
	VOID,
	AND,
	OR,
	PIPE,
	LINE_CHANGE
}	t_operator;

typedef enum s_switch
{
	OFF,
	ON
}	t_switch;

typedef struct s_leaf
{
	t_list *tokens;
	t_switch handle_here_doc;
	int fd_input;
	int fd_output;
	int returned_value;
	t_switch parenthesis;
	t_operator ope_after;
}	t_leaf;

typedef struct s_AST_node
{
	t_node_type type;
	union
	{
		t_leaf *command;
		struct
		{
			t_operator control_operator;
			struct s_AST_node *left_node;
			struct s_AST_node *right_node;
		} t_ope_node;
	};
}	t_AST_node;


void	create_tokens(t_list **tokens, char *input);
t_leaf *create_cmd_tab(t_list *tokens);
char	*ft_strjoin_alt(char *s1, char *s2);
void	handle_all_here_doc(t_leaf *command_tab);
t_AST_node	*create_ast(t_leaf *command_tab);
t_leaf	*evaluate_ast(t_AST_node *node);
int	execute_cmd(t_leaf *cmd);
void	rm_here_doc_files(t_leaf *command_tab);



#endif
