/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:40:07 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/17 20:12:27 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	make_var_separator(char *str, long separator, t_minishell *data);
static bool	print_export_error(char *str);

int	export_builtin(int argc, char **argv, t_minishell *data)
{
	int			code;
	long		separator;
	size_t		index;

	if (argc <= 1)
		return (env_builtin(data));
	index = 0;
	code = EXIT_SUCCESS;
	while (argv[++index])
	{
		separator = ft_strchri(argv[index], '=');
		if (argv[index][0] == '=' || \
			separator <= 0 || \
			(unsigned long) separator == ft_strlen(argv[index]) - 1 || \
			(!ft_isalpha(argv[index][0]) && argv[index][0] != '_'))
		{
			if (!print_export_error(argv[index]) && code != EXIT_FAILURE)
				return (BUILTIN_FATAL_ERROR);
			code = EXIT_FAILURE;
			continue ;
		}
		if (!make_var_separator(argv[index], separator, data))
			return (BUILTIN_FATAL_ERROR); //EXIT MINISHELL IF THAT HAPPEN
	}
	return (code);
}

static bool	print_export_error(char *str)
{
	char	*temp;
	long	separator;

	separator = ft_strchri(str, '=');
	if (separator == -1 || separator == 0)
		temp = ft_substr(str, 0, ft_strlen(str));
	else
		temp = ft_substr(str, 0, separator);
	if (!temp)
		return (false);
	ft_putstr_fd("Export : Not a valid identifier : ", 2);
	ft_putstr_fd(temp, 2);
	ft_putchar_fd('\n', 2);
	free(temp);
	return (true);
}

static bool	make_var_separator(char *str, long separator, t_minishell *data)
{
	char		*key;
	char		*value;
	t_envvar	*var;

	key = ft_substr(str, 0, separator);
	value = ft_substr(str, separator + 1, ft_strlen(str));
	if (!value || !key)
	{
		if (key)
			free(key);
		if (value)
			free(value);
		return (false);
	}
	var = new_envvar(key, value);
	if (hashmap_insert(hash(key), var, &data->environment) == 0)
		return (false);
	return (true);
}
