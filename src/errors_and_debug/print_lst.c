/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:56:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/14 21:31:33 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_lst(t_lst	*lst, t_lst_members members)
{
	while (lst)
	{
		if (members == LEXEME)
			ft_printf_fd(2, "   lexeme :%s\n", lst->lexeme);
		if (members == (LEXEME | TYPE))
		{
			ft_printf_fd(2, "   lexeme :%s\n", lst->lexeme);
			ft_printf_fd(2, "   type :%d\n", lst->type);
		}
		lst = lst->next;
	}
}

void	print_debug_lst(t_lst *lst, t_lst_members members, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(2, "%s", str);
		print_lst(lst, members);
	}
}
