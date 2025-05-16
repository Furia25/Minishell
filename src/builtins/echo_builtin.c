/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:15:50 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/16 18:59:53 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_flags(char *str);

int	echo_builtin(int argc, char **argv)
{
	bool	do_nl;
	int		index;

	do_nl = true;
	index = 1;
	while (index < argc)
	{
		if (is_valid_flags(argv[index]))
			do_nl = false;
		else
		{
			if (!write_str_secure(argv[index], 1))
				return (EXIT_FAILURE);
			if (index < argc - 1 && write(1, " ", 1) == -1)
				return (EXIT_FAILURE);
		}
		index++;
	}
	if (do_nl)
		if (write(1, "\n", 1) == -1)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static bool	is_valid_flags(char *str)
{
	if (!str)
		return (false);
	if (*str != BUILTIN_FLAG_SEPARATOR)
		return (false);
	str++;
	while (str && *str)
	{
		if (*str != BUILTIN_FLAG_ECHO_NONL)
			return (false);
		str++;
	}
	return (true);
}
