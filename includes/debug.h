/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:01:44 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/15 19:49:25 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*0: default value, no display
1: display all debug message
2: display tokens just after creating it
3: display command_tab just after creating it
4: display input in here doc after handle ev_expension and subshell in it
5: display command_tab after handle here doc (change node type)
6: display AST just after creating it 
7: display command->tokens after handle ev_expension and subshell
8: display command->tokens after handle fusion quotes
9: display command->tokens after handle wildcards (add wilcards' type nodes in command->tokens)
10: display command after handle redi (rm redi's type nodes of command->tokens and change fd values)
11: display argv after creating it (not adding WORD tokens whose lexeme is null-terminated)*/

#ifndef DEBUG_H
# define DEBUG_H
# include "parsing.h"
# include "ft_printf.h"

#ifndef DEBUG
# define DEBUG 11
#endif

typedef enum e_lst_members
{
	LEXEME = 1<<0,
	TYPE = 1<<1
}	t_lst_members;

void	print_debug_lst(t_lst *lst, t_lst_members members, int debug, char *str);
void	print_debug_cmd(t_leaf *command, t_lst_members cmd_tokens_members, int debug, char *str);
void	print_debug_all_cmd(t_leaf *command_tab, t_lst_members cmd_tokens_members, int debug, char *str);
void	print_debug_ast(t_AST_node *top_node_ast, int debug, char *str);
void	print_debug_argv(char **tab, int debug, char *str);

#endif
