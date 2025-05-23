/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:02:42 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/21 20:23:25 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_lst(t_lst	*lst, t_lst_members members);

static void	print_cmd(t_leaf *command, t_lst_members cmd_tokens_members)
{
	ft_printf_fd(STDERR_FILENO, "\n\nnew cmd :\n\n");
	ft_printf_fd(STDERR_FILENO, "tokens: \n");
	print_lst(command->tokens, cmd_tokens_members);
	ft_printf_fd(STDERR_FILENO, "fd_in :%d\n", command->fd_input);
	ft_printf_fd(STDERR_FILENO, "fd_out :%d\n", command->fd_output);
	ft_printf_fd(STDERR_FILENO, "parenthesis :%d\n", command->parenthesis);
	ft_printf_fd(STDERR_FILENO, "ope_after :%d\n", command->ope_after);
}

static void	print_all_cmd(t_leaf *command_tab, t_lst_members cmd_tokens_members)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		print_cmd(command_tab, cmd_tokens_members);
		command_tab++;
	}
	print_cmd(command_tab, cmd_tokens_members);
}

void	print_debug_cmd(t_leaf *command,
	t_lst_members cmd_tokens_members, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(STDERR_FILENO, "%s", str);
		print_cmd(command, cmd_tokens_members);
	}
}

void	print_debug_all_cmd(t_leaf *command_tab,
	t_lst_members cmd_tokens_members, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(STDERR_FILENO, "%s", str);
		print_all_cmd(command_tab, cmd_tokens_members);
	}
}
