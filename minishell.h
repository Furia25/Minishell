/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/20 13:07:09 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/incs/libft.h"
# include <stdio.h>
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

typedef enum s_control_operator
{
	VOID,
	AND,
	OR,
	AND_or_OR,
	PIPE,
	LINE_CHANGE
}	t_control_operator;

typedef struct s_leaf
{
	t_list *tokens;
	char cmd_stdin;
	char cmd_stdout;
	int returned_value;
	t_control_operator ope_after;
}	t_leaf;

typedef struct s_AST_node
{
	t_node_type type;
	union
	{
		t_leaf *command;
		struct
		{
			t_control_operator control_operator;
			struct s_AST_node *left_node;
			struct s_AST_node *right_node;
		} t_ope_node;
	};
}	t_AST_node;


char	*create_tokens(t_list **tokens, char *input);
t_leaf *create_cmd_tab(t_list *tokens);
t_AST_node	*create_ast(t_leaf *command_tab);
t_leaf	*evaluate_ast(t_AST_node *node);



#endif
