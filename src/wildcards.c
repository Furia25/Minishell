/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:49:08 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/08 20:42:29 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "wildcards.h"
#include "libft.h"
#include "stdlib.h"
#include <stdint.h>

t_list	*wildcard_search(char *token);
t_bool matches(const char *name, const char *pattern);
t_list	*wildcard_explore(char *base_dir, t_wildcard *wildcard, int depth);

t_list	*wildcard_search(char *token)
{
	t_wildcard	wildcard;
	size_t		index;

	wildcard.component = ft_split(token, PATH_SYMBOL);
	if (!wildcard.component)
		return (NULL);
	index = 0;
	wildcard.count = 0;
	while (wildcard.component[index] != '\0')
		wildcard.count += 1;
	wildcard.result = NULL;
	wildcard.token = token;
	wildcard_explore(".", &wildcard, 0);
	free_chartab(wildcard.component);
	return (wildcard.result);
}

t_bool wildcard_matches(const char *name, const char *pattern)
{
	const char *name_ptr = name;
	const char *pattern_ptr = pattern;
	const char *backtrack_name = NULL;
	const char *backtrack_pattern = NULL;

	while (*name_ptr != '\0')
	{
		if (*pattern_ptr == '*')
		{
			backtrack_name = name_ptr;
			backtrack_pattern = ++pattern_ptr;
		} else if (*pattern_ptr == *name_ptr || *pattern_ptr == '?')
		{
			name_ptr++;
			pattern_ptr++;
		} else if (backtrack_pattern != NULL)
		{
			name_ptr = ++backtrack_name;
			pattern_ptr = backtrack_pattern;
		} else
			return FALSE;
	}
	while (*pattern_ptr == '*')
		pattern_ptr++;
	return *pattern_ptr == '\0';
}

int read_directory(DIR *dir, struct dirent **entry)
{
	*entry = readdir(dir);
	if (*entry == NULL)
		return (0);
	return (1);
}

t_bool	check_special_entry(char *name)
{
	return (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0);
}

char	*get_file_name(char *dir_name, char *file_name)
{
	size_t	index;
	size_t	length_dir;
	size_t	lenght_file;
	char	*result;
	
	length_dir = ft_strlen(dir_name);
	lenght_file = ft_strlen(file_name);
	result = ft_calloc(length_dir + lenght_file + 2, sizeof(char));
	if (!result)
		return (NULL);
	ft_strcpy(result, dir_name);
	ft_strcat(result, "/");
	ft_strcat(result, file_name);
	return (result);
}

t_list	*wildcard_explore(char *dir_name, t_wildcard *wildcard, int depth)
{
	DIR				*dir;
	t_list			*results;
	t_list			*sub_results;
	struct dirent	*entry;
	char			*new_path;
	
	dir = opendir(dir_name);
	if (!dir)
		return (NULL);
	results = NULL;
	while (read_directory(dir, &entry))
	{
		if (check_special_entry(entry->d_name))
			continue;
		if (!wildcard_matches(entry->d_name, wildcard->component[depth]))
			continue;
		new_path = get_file_name(dir_name, entry->d_name);
		if (!new_path)
		{
			ft_lstclear(results, free);
			closedir(dir);
			return (NULL);
		}
		if (depth + 1 < wildcard->count)
		{
			sub_results = wildcard_explore(new_path, wildcard, depth + 1);
			if (sub_results)
			{
				if (!results)
					results = sub_results;
				else
					ft_lstadd_allback(&results, sub_results);
			}
			else
			{
				
			}
		}
	}
	closedir(dir);
	return (results);
}

int	main(int argc, char **argv)
{
	wildcard_search("*");
}