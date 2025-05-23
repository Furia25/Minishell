/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:49:08 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 02:21:02 by val              ###   ########.fr       */
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

	wildcard.rules = ft_split(lexeme, PATH_SYMBOL);
	if (!wildcard.rules)
		return ((t_wsearch){-1, NULL});
	wildcard.count = 0;
	while (wildcard.rules[wildcard.count] != NULL)
		wildcard.count += 1;
	wildcard.result = NULL;
	wildcard.lexeme = lexeme;
	if (*lexeme == '/')
		search_result = wildcard_explore("/", &wildcard, 0);
	else
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
	size_t	length_total;
	char	*result;

	if (ft_strcmp(dir_name, ".") == 0)
		return (ft_strdup(file_name));
	length_dir = ft_strlen(dir_name);
	length_total = length_dir + ft_strlen(file_name) + 3;
	result = ft_calloc(length_total, sizeof(char));
	if (!result)
		return (NULL);
	ft_strcpy(result, dir_name);
	if (dir_name[length_dir - 1] != '/')
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
