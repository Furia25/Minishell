/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:56:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/20 23:01:26 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_lst(t_lst	*lst, t_lst_members members)
{
	while (lst)
	{
		ft_printf("   lexeme :%s\n", lst->lexeme);
		if (members == LEXEME_AND_TYPE)
			ft_printf("   type: %d\n", lst->type);
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
