/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:22:24 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/26 18:10:43 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include "builtin.h"

static void			exit_and_print(t_minishell *data);
static void			exit_parse_value(char *value, bool sign, t_minishell *data);
static long long	exit_atoi(const char *nptr);

int	exit_builtin(int argc, char **argv, t_minishell *data)
{
	char			*value;
	bool			negative;

	if (argc > 2)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: too many arguments\n",
			MINISHELL_NAME, BUILTIN_NAME_EXIT);
		data->exit_code = 2;
		exit_and_print(data);
	}
	if (argc == 1)
		exit_and_print(data);
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

static void	exit_parse_value(char *value, bool sign, t_minishell *data)
{
	size_t	index;
	size_t	digits;

	index = 0;
	digits = ft_count_digits(LLONG_MAX);
	while (value[index])
	{
		if (!ft_isdigit(value[index]) || digits <= 0)
		{
			ft_printf_fd(STDERR_FILENO, "%s: %s: %s: numeric argument \
required\n", MINISHELL_NAME, BUILTIN_NAME_EXIT, value);
			data->exit_code = 2;
			exit_and_print(data);
		}
		digits--;
		index++;
	}
	data->exit_code = exit_atoi(value) * (1 - 2 * sign);
	exit_and_print(data);
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

static void	exit_and_print(t_minishell *data)
{
	if (isatty(STDOUT_FILENO) == 1
		&& isatty(STDIN_FILENO) == 1
		&& !data->script_mode)
	{
		ft_putstr_fd("\033[31m", 2);
		ft_putstr_fd(BUILTIN_EXIT_MESSAGE, STDERR_FILENO);
		ft_putstr_fd("\033[0m", 2);
	}
	exit_minishell(data);
}
