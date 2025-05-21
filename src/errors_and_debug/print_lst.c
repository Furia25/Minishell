/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:56:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/21 20:23:41 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_lst(t_lst	*lst, t_lst_members members)
{
	while (lst)
	{
		if (members == LEXEME)
			ft_printf_fd(STDERR_FILENO, "   lexeme :%s\n", lst->lexeme);
		if (members == (LEXEME | TYPE))
		{
			ft_printf_fd(STDERR_FILENO, "   lexeme :%s\n", lst->lexeme);
			ft_printf_fd(STDERR_FILENO, "   type :%d\n", lst->type);
		}
		lst = lst->next;
	}
}

void	print_lst_one_line(t_lst *lst)
{
	while (lst)
	{
		ft_printf_fd(STDERR_FILENO, "%s ", lst->lexeme);
		lst = lst->next;
	}
	ft_putchar_fd('\n', STDERR_FILENO);
}

void	print_debug_lst(t_lst *lst, t_lst_members members, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(STDERR_FILENO, "%s", str);
		print_lst(lst, members);
	}
}
