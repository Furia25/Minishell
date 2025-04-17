/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:15:50 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/18 01:17:39 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	write_str_secure(char *str);
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
			if (!write_str_secure(argv[index]))
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

static bool	write_str_secure(char *str)
{
	while (str && *str)
	{
		if (write(1, str, 1) == -1)
			return (false);
		str++;
	}
	return (true);
}
