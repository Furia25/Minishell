/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:22:24 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/16 19:35:11 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	exit_parse_value(char *value, unsigned char *result);

int	exit_builtin(int argc, char **argv, t_minishell *data)
{
	char			*value;
	unsigned char	result;

	if (argc > 2)
	{
		ft_putstr_fd("minishell", 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(BUILTIN_ERROR_EXIT, 2);
		ft_putchar_fd('\n', 2);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		exit_minishell(data);
	value = argv[1];
	result = 0;
	while (value && *value == '0')
		value++;
	if (value)
	{
		if (!exit_parse_value(value, &result))
			return (EXIT_FAILURE);
	}
	data->exit_code = result;
	exit_minishell(data);
	return (EXIT_SUCCESS);
}

static bool	exit_parse_value(char *value, unsigned char *result)
{
	size_t	index;

	index = 0;
	while (value[index])
	{
		if (!ft_isdigit(value[index]))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(value, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (false);
		}
		index++;
	}
	*result = ft_atoi(value);
	return (true);
}
