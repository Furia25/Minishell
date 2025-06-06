/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_structs.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:56:49 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/30 19:03:46 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCTS_H
# define MINISHELL_STRUCTS_H
# include "hashmap.h"

typedef enum e_lexeme_type
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
	LAST_PAR_CLOSE,
	AND,
	OR,
	PIPE,
	DOLLAR,
	WILDCARD,
	LINE_CHANGE
}	t_lexeme_type;

typedef struct s_lst
{
	char			*lexeme;
	t_lexeme_type	type;
	bool			metacharacter_after;
	struct s_lst	*next;
}	t_lst;

typedef struct s_leaf
{
	t_lst			*tokens;
	int				fd_input;
	int				fd_output;
	int				returned_value;
	bool			parenthesis;
	t_lexeme_type	ope_after;
}	t_leaf;

typedef struct s_minishell
{
	t_hashmap			gc;
	t_hashmap			environment;
	bool				welcome_start;
	char				**environment_tab;
	char				*script_file;
	size_t				line;
	int					last_cmd_pid;
	int					script_fd;
	bool				script_mode;
	t_leaf				*command_tab;
	bool				in_child;
	bool				in_pipe;
	unsigned char		exit_code;
	bool				use_hard_path;
	int					echoctl_base_value;
}	t_minishell;

#endif