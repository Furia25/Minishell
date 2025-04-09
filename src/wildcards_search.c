/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_search.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:18:29 by val               #+#    #+#             */
/*   Updated: 2025/04/09 01:55:08 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "wildcards.h"
#include "libft.h"
#include "stdlib.h"
#include <stdint.h>

static bool			switch_rec_results(t_list **results, t_wsearch search);
static char			*get_file_name(char *dir_name, char *file_name);
static t_wsearch	clean_return(t_list *results, DIR *dir, char *path);
static int			read_directory(DIR *dir, struct dirent **entry);
static bool			check_special_entry(char *name);

t_wsearch	wildcard_explore(char *dir_name, t_wildcard *wildcard, size_t depth)
{
	DIR				*dir;
	t_list			*results;
	t_list			*new;
	struct dirent	*entry;
	char			*new_path;
	
	dir = opendir(dir_name);
	if (!dir)
		return ((t_wsearch) {0, NULL});
	results = NULL;
	while (read_directory(dir, &entry))
	{
		if (check_special_entry(entry->d_name) || \
			!wildcard_matches(entry->d_name, wildcard->component[depth]))
			continue;
		new_path = get_file_name(dir_name, entry->d_name);
		if (!new_path)
			return (clean_return(results, dir, NULL));
		if (depth + 1 < wildcard->count)
		{
			if (!switch_rec_results(&results, \
				wildcard_explore(new_path, wildcard, depth + 1)))
				return (clean_return(results, dir, new_path));
			free(new_path);
		}
		else
		{
			new = ft_lstnew(new_path);
			if (new)
				ft_lstadd_back(&results, new);
			else
				return (clean_return(results, dir, new_path));
		}
	}
	closedir(dir);
	return ((t_wsearch) {results != NULL, results});
}

static bool	switch_rec_results(t_list **results, t_wsearch search_result)
{
	if (search_result.code == 1)
	{
		if (!(*results))
			*results = search_result.result;
		else
			ft_lstadd_back(results, search_result.result);
		return (true);
	}
	else if (search_result.code == 0)
		return (true);
	return (false);
}

static int	read_directory(DIR *dir, struct dirent **entry)
{
	*entry = readdir(dir);
	if (*entry == NULL)
		return (0);
	return (1);
}

static bool	check_special_entry(char *name)
{
	return (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0);
}

static char	*get_file_name(char *dir_name, char *file_name)
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

static t_wsearch	clean_return(t_list *results, DIR *dir, char *path)
{
	if (path)
		free(path);
	if (results)
		ft_lstclear(&results, free);
	if (dir)
		closedir(dir);
	return ((t_wsearch) {-1, NULL});
}
