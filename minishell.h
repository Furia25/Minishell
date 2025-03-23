/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/23 02:58:29 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/incs/libft.h"
# include <stdio.h>
#include <fcntl.h>

# include <stdlib.h>
// # include <readline/readline.h>
// # include <readline/history.h>

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
	RED_I,
	RED_O,
	LINE_CHANGE
}	t_operator;

typedef struct s_leaf
{
	t_list *tokens;
	int fd_input;
	int fd_output;
	int returned_value;
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
t_AST_node	*create_ast(t_leaf *command_tab);
t_leaf	*evaluate_ast(t_AST_node *node);



#endif
