/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_search.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:18:29 by val               #+#    #+#             */
/*   Updated: 2025/05/20 17:42:12 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "wildcards.h"
#include "libft.h"
#include "stdlib.h"
#include <stdint.h>

static bool			switch_depth_handler(char *new_path, t_wildcard *wd,
						size_t depth, t_lst **results);
static bool			handle_recursive_case(char *new_path, t_wildcard *wd,
						size_t depth, t_lst **results);
static t_wsearch	clean_return(t_lst *results, DIR *dir, char *path);
static bool			should_skip_entry(char *name, t_wildcard *wd, size_t depth);

t_wsearch	wildcard_explore(char *dir_name, t_wildcard *wd, size_t depth)
{
	DIR				*dir;
	t_lst			*results;
	struct dirent	*file;
	char			*new_path;

	dir = opendir(dir_name);
	if (!dir)
		return ((t_wsearch){0, NULL});
	results = NULL;
	while (read_directory(dir, &file))
	{
		if (should_skip_entry(file->d_name, wd, depth))
			continue ;
		new_path = get_dirfile_name(dir_name, file->d_name);
		if (!new_path)
			return (clean_return(results, dir, NULL));
		if (!switch_depth_handler(new_path, wd, depth, &results))
			return (clean_return(results, dir, NULL));
	}
	closedir(dir);
	return ((t_wsearch){results != NULL, results});
}

static bool	switch_depth_handler(char *new_path, t_wildcard *wd,
	size_t depth, t_lst **results)
{
	t_lst	*new;

	if (depth + 1 < wd->count)
	{
		return (handle_recursive_case(new_path, wd, depth, results));
	}
	else
	{
		new = lstnew(new_path);
		if (!new)
			return (false);
		new->type = WILDCARD;
		lstadd_back(results, new);
		return (true);
	}
}

static bool	handle_recursive_case(char *new_path, t_wildcard *wd,
	size_t depth, t_lst **results)
{
	t_wsearch	search_result;

	search_result = wildcard_explore(new_path, wd, depth + 1);
	if (search_result.code == 1)
	{
		if (!(*results))
			*results = search_result.result;
		else
			lstadd_back(results, search_result.result);
		free(new_path);
		return (true);
	}
	else if (search_result.code == 0)
	{
		free(new_path);
		return (true);
	}
	free(new_path);
	return (false);
}

static bool	should_skip_entry(char *name, t_wildcard *wd, size_t depth)
{
	size_t	len;
	char	*p;
	bool	show_hidden;

	p = wd->rules[depth];
	len = ft_strlen(p);
	show_hidden = (p[0] == '.' || ft_strnstr(p, "/.", len));
	if (name[0] == '.' && !show_hidden)
		return (true);
	return (!wildcard_matches(name, p));
}

static t_wsearch	clean_return(t_lst *results, DIR *dir, char *path)
{
	if (path)
		free(path);
	if (results)
		lstclear(&results, free);
	if (dir)
		closedir(dir);
	return ((t_wsearch){-1, NULL});
}
