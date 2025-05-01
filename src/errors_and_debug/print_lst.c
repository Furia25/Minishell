/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:56:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/01 18:27:35 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_lst(t_lst	*lst, t_lst_members members)
{
	while (lst)
	{
		if (members == LEXEME)
			ft_printf("   lexeme :%s\n", lst->lexeme);
		if (members == (LEXEME | TYPE))
		{
			ft_printf("   lexeme :%s\n", lst->lexeme);
			ft_printf("   type :%d\n", lst->type);
		}
		if (members == (LEXEME | TYPE | SPECIAL_PARAMETER))
		{
			ft_printf("   lexeme :%s\n", lst->lexeme);
			ft_printf("   type :%d\n", lst->type);
			ft_printf("   special_para :%d\n", lst->special_parameter);
		}
		lst = lst->next;
	}
}

void	print_debug_lst(t_lst *lst, t_lst_members members, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf("%s", str);
		print_lst(lst, members);
	}
}
