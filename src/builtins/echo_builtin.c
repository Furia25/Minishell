/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:15:50 by vdurand           #+#    #+#             */
/*   Updated: 2025/06/02 11:31:18 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

static bool	is_valid_flag(char *str);

int	echo_builtin(int output, int argc, char **argv)
{
	bool	has_n_flag;
	int		i;

	has_n_flag = false;
	i = 1;
	while (i < argc && is_valid_flag(argv[i]))
	{
		has_n_flag = true;
		i++;
	}
	while (i < argc)
	{
		ft_putstr_fd(argv[i], output);
		if (i < argc - 1)
			write(output, " ", 1);
		i++;
	}
	if (!has_n_flag)
		write(output, "\n", 1);
	return (EXIT_SUCCESS);
}

static bool	is_valid_flag(char *str)
{
	if (!str || *str != '-')
		return (false);
	str++;
	if (*str == '\0')
		return (false);
	while (*str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}
