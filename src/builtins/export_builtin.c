/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:40:07 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 15:19:16 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "environment.h"

static bool	make_var_separator(char *str, long separator, t_minishell *data);
static int	print_export_error(char *str);
static bool	is_arg_valid(char *str);

int	export_builtin(int output, int argc, char **argv, t_minishell *data)
{
	int			code;
	long		separator;
	size_t		index;

	if (argc <= 1)
	{
		env_print(output, &data->environment, true);
		return (EXIT_SUCCESS);
	}
	index = 0;
	code = EXIT_SUCCESS;
	while (argv[++index])
	{
		separator = ft_strchri(argv[index], '=');
		if (is_arg_valid(argv[index]))
		{
			if (print_export_error(argv[index]) == -1 && code != EXIT_FAILURE)
				return (BUILTIN_FATAL_ERROR);
			code = EXIT_FAILURE;
			continue ;
		}
		if (!make_var_separator(argv[index], separator, data))
			return (BUILTIN_FATAL_ERROR);
	}
	return (code);
}

static bool	is_arg_valid(char *str)
{
	return (str[0] == '=' || (!ft_isalpha(str[0]) && str[0] != '_'));
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
	ft_printf_fd(STDERR_FILENO, "%s: %s: Not a valid identifier: %s\n",
		MINISHELL_NAME, BUILTIN_NAME_EXPORT, temp);
	free(temp);
	return (1);
}

static bool	make_var_separator(char *str, long separator, t_minishell *data)
{
	char		*key;
	char		*value;
	t_envvar	*var;
	size_t		size;

	size = ft_strlen(str);
	if (separator == -1)
		separator = size;
	key = ft_substr(str, 0, separator);
	value = ft_substr(str, separator + 1, size);
	if (!value || !key)
	{
		free(key);
		free(value);
		return (false);
	}
	var = new_envvar(key, value);
	if (separator == (long) size)
		var->exported = false;
	if (!hashmap_insert(hash(key), var, &data->environment))
	{
		envvar_free(var);
		return (false);
	}
	return (true);
}
