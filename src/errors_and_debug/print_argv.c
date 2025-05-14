/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:18:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/14 21:33:27 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

static void	print_argv(char **tab)
{
	ft_printf_fd(2, "\n\nnew argv :\n\n");
	while (*tab)
	{
		ft_printf_fd(2, "argv :%s\n", *tab);
		tab++;
	}
}

void	print_debug_argv(char **tab, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(2, "%s", str);
		print_argv(tab);
	}
}
