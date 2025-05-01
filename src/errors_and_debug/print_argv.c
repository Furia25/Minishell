/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:18:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/01 18:27:07 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

static void	print_argv(char **tab)
{
	ft_printf("\n\nnew argv :\n\n");
	while (*tab)
	{
		ft_printf("argv :%s\n", *tab);
		tab++;
	}
}

void	print_debug_argv(char **tab, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf("%s", str);
		print_argv(tab);
	}
}
