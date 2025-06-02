/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:48:45 by vdurand           #+#    #+#             */
/*   Updated: 2025/06/02 12:01:14 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_H
# define WILDCARDS_H
# include "libft.h"
# include "parsing.h"
# include <stddef.h>
# include <dirent.h>

# define WILDCARD_SYMBOL	'*'
# define PATH_SYMBOL	'/'

typedef struct s_wildcard
{
	char	**rules;
	t_lst	*result;
	size_t	count;
	char	*lexeme;
}	t_wildcard;

typedef struct s_wsearch
{
	int		code;
	t_lst	*result;
}	t_wsearch;

void		wildcards_in_cmd(t_leaf *cmd, t_minishell *data);
t_wsearch	wildcard_explore(char *dir_name, t_wildcard *wd, size_t depth);
t_wsearch	wildcard_lst_from_lexeme(char *lexeme);
bool		wildcard_matches(char *name_ptr, char *pattern_ptr);
char		*get_dirfile_name(char *dir_name, char *file_name);
int			read_directory(DIR *dir, struct dirent **entry);

#endif
