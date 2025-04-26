/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:01:44 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 04:59:35 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*0: default value, no display
1: display all debug message
2: display tokens just after creating it
3: display command_tab just after creating it
4: display input in here doc after handle ev_expension and subshell
5: display command_tab after handle here doc (change node type )
6: display command->tokens after handle ev_expension and subshell
7: display command->tokens after handle fusion quotes
8: display command after handle redi (rm some nodes of command->tokens and change fd values)*/

#ifndef DEBUG_H
# define DEBUG_H
# include "minishell.h"

#ifndef DEBUG
# define DEBUG 7
#endif

typedef enum s_lst_members
{
	ONLY_LEXEME,
	LEXEME_AND_TYPE
}	t_lst_members;

void	print_debug_lst(t_lst *lst, t_lst_members members, int debug, char *str);
void	print_debug_cmd(t_leaf *command, t_lst_members cmd_tokens_members, int debug, char *str);
void	print_debug_all_cmd(t_leaf *command_tab, t_lst_members cmd_tokens_members, int debug, char *str);
void	print_debug_argv(char **tab, int debug, char *str);

#endif
