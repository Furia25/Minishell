/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:15:50 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/17 20:41:34 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_builtin(int argc, char **argv)
{
	bool	do_nl;
	char	*str;

	do_nl = true;
	while (argc > 1)
	{
		str = argv[argc];
		if (argc == 2 && ft_strcmp(str, BUILTIN_ECHO_FLAG) == 0)
		{
			do_nl = false;
			ft_printf("fdgfdgfdgfd");
		}
		else
		{
			while (str && *str)
			{
				if (write(1, str, 1) == -1)
					return (EXIT_FAILURE);
				str++;
			}
		}
		argc--;
	}
	if (do_nl)
		if (write(1, "\n", 1) == -1)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
