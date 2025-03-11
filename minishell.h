/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/11 15:10:06 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/incs/libft.h"
# include <stdio.h>
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

typedef enum s_control_operator
{
	AND,
	OR,
	PIPE,
	LINE_CHANGE
}	t_control_operator;

typedef struct s_leaf
{
	char tokens;
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
			int control_operator;
			struct s_AST_node *left_node;
			struct s_AST_node *right_node;
		} t_ope_node;
	};
}	t_AST_node;

#endif