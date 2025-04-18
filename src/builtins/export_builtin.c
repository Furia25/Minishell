/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:40:07 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/18 02:32:46 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	make_var_separator(char *str, long separator, t_minishell *data);
static int	print_export_error(char *str);

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
			if (print_export_error(argv[index]) == -1 && code != EXIT_FAILURE)
				return (BUILTIN_FATAL_ERROR);
			code = EXIT_FAILURE;
			continue ;
		}
		if (!make_var_separator(argv[index], separator, data))
			return (BUILTIN_FATAL_ERROR); //EXIT MINISHELL IF THAT HAPPEN
	}
	return (code);
}

static int	print_export_error(char *str)
{
	char	*temp;
	long	separator;

	separator = ft_strchri(str, '=');
	if (separator == -1 || separator == 0)
		temp = ft_substr(str, 0, ft_strlen(str));
	else
		temp = ft_substr(str, 0, separator);
	if (!temp)
		return (-1);
	if (!write_str_secure(BUILTIN_ERROR_EXPORT, 2) || !write_str_secure(temp, 2))
	{
		free(temp);
		return (0);
	}
	free(temp);
	if (write(2, "\n", 1) == -1)
		return (0);
	return (1);
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
