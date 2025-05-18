/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:49:08 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/18 23:58:52 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "wildcards.h"
#include "libft.h"
#include "stdlib.h"
#include <stdint.h>

static bool	pattern(char **name, char **pattern,
				char **bt_name, char **bt_pattern);

t_wsearch	wildcard_lst_from_lexeme(char *lexeme)
{
	t_wildcard	wildcard;
	t_wsearch	search_result;
	size_t		index;

	wildcard.rules = ft_split(lexeme, PATH_SYMBOL);
	if (!wildcard.rules)
		return ((t_wsearch){-1, NULL});
	index = 0;
	wildcard.count = 0;
	while (wildcard.rules[index] != NULL)
	{
		wildcard.count += 1;
		index++;
	}
	wildcard.result = NULL;
	wildcard.lexeme = lexeme;
	search_result = wildcard_explore(".", &wildcard, 0);
	free_chartab(wildcard.rules);
	return (search_result);
}

bool	wildcard_matches(char *name_ptr, char *pattern_ptr)
{
	char	*bt_name;
	char	*bt_pattern;

	bt_name = NULL;
	bt_pattern = NULL;
	while (*name_ptr != '\0')
		if (!pattern(&name_ptr, &pattern_ptr, &bt_name, &bt_pattern))
			return (false);
	while (*pattern_ptr == '*')
		pattern_ptr++;
	return (*pattern_ptr == '\0');
}

static bool	pattern(char **n, char **p, char **bt_n, char **bt_p)
{
	if (**p == '*')
	{
		*bt_n = *n;
		*bt_p = ++(*p);
		return (true);
	}
	if (**p == **n || (**p == '?')
		|| (**p == (- '*') && **n == '*')
		|| (**p == (- '?') && **n == '?'))
	{
		(*n)++;
		(*p)++;
		return (true);
	}
	if (*bt_p)
	{
		*n = ++(*bt_n);
		*p = *bt_p;
		return (true);
	}
	return (false);
}

char	*get_dirfile_name(char *dir_name, char *file_name)
{
	size_t	length_dir;
	size_t	length_file;
	char	*result;

	if (ft_strcmp(dir_name, ".") == 0)
		return (ft_strdup(file_name));
	length_dir = ft_strlen(dir_name);
	length_file = ft_strlen(file_name);
	result = ft_calloc(length_dir + length_file + 2, sizeof(char));
	if (!result)
		return (NULL);
	ft_strcpy(result, dir_name);
	ft_strcat(result, "/");
	ft_strcat(result, file_name);
	return (result);
}

int	read_directory(DIR *dir, struct dirent **entry)
{
	*entry = readdir(dir);
	if (*entry == NULL)
		return (0);
	return (1);
}
