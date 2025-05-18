/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:22:24 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/18 23:37:39 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static void			exit_parse_value(char *value, bool is_negative, t_minishell *data);
static long long	exit_atoi(const char *nptr);

int	exit_builtin(int argc, char **argv, t_minishell *data)
{
	char			*value;
	bool			negative;

	if (argc > 2)
	{
		ft_putstr_fd("minishell", 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(BUILTIN_ERROR_EXIT, 2);
		ft_putchar_fd('\n', 2);
		data->exit_code = 2;
		exit_minishell(data);
	}
	if (argc == 1)
		exit_minishell(data);
	value = argv[1];
	negative = false;
	if (value && *value == '-')
	{
		value++;
		negative = true;
	}
	while (value && *value == '0')
		value++;
	exit_parse_value(value, negative, data);
	return (EXIT_SUCCESS);
}

static void	exit_parse_value(char *value, bool is_negative, t_minishell *data)
{
	size_t	index;
	size_t	digits;

	index = 0;
	digits = ft_count_digits(LLONG_MAX);
	while (value[index])
	{
		if (!ft_isdigit(value[index]) || digits <= 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(value, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			data->exit_code = 2;
			exit_minishell(data);
		}
		digits--;
		index++;
	}
	data->exit_code = exit_atoi(value) * (1 - 2 * is_negative);
	exit_minishell(data);
}

static long long	exit_atoi(const char *nptr)
{
	int					index;
	int					sign;
	unsigned long long	result;

	if (!nptr)
		return (0);
	index = 0;
	result = 0;
	sign = 1;
	while (nptr[index] && nptr[index] >= '0' && nptr[index] <= '9')
	{
		result = result * 10 + (nptr[index] - '0');
		index++;
	}
	return (result * sign);
}
