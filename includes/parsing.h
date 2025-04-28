/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:23:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 15:55:00 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "minishell.h"
# include "parsing.h"

typedef enum s_lexeme_type
{
	VOID,
	WORD,
	SINGLE_Q,
	DOUBLE_Q,
	RED_IN,
	RED_OUT,
	RED_IN_OUT,
	RED_OUT_A,
	HERE_DOC,
	PAR_OPEN,
	PAR_CLOSE,
	AND,
	OR,
	PIPE,
	SUBSHELL,
	LINE_CHANGE
}	t_lexeme_type;

typedef struct s_lst
{
	char *lexeme;
	t_lexeme_type type;
	bool metacharacter_after;
	struct s_lst *next;
}	t_lst;

typedef struct s_leaf
{
	t_lst *tokens;
	int fd_input;
	int fd_output;
	int returned_value;
	bool parenthesis;
	t_lexeme_type ope_after;
}	t_leaf;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_OPERATOR_CONTROL
}	t_node_type;

typedef struct s_AST_node
{
	t_node_type	type;
	union
	{
		t_leaf	*command;
		struct
		{
			t_lexeme_type control_operator;
			struct s_AST_node *left_node;
			struct s_AST_node *right_node;
		} t_ope_node;
	};
}	t_AST_node;

t_lst	*lstnew(char *lexeme);
t_lst	*lstlast(t_lst *lst);
void	lstadd_back(t_lst **lst, t_lst *new);
void	lstdelone(t_lst *lst, void (*del)(void*));
void	lstclear(t_lst **lst, void (*del)(void*));

t_AST_node	*create_ast(t_leaf *command_tab, t_minishell *data);
t_leaf	*evaluate_ast(t_AST_node *node, t_minishell *data);

#endif