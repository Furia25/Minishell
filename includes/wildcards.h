/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:48:45 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/09 01:37:07 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_H
# include "libft.h"
# include <stddef.h>
# include <dirent.h>

# define WILDCARD_SYMBOL	'*'
# define PATH_SYMBOL	'/'

typedef struct s_wildcard
{
	char	**component;
	t_list	*result;
	size_t	count;
	char	*token;
}	t_wildcard;

typedef struct s_wsearch
{
	int		code;
	t_list	*result;
}	t_wsearch;

t_wsearch	wildcard_explore(char *dir_name, t_wildcard *wildcard, size_t depth);
t_wsearch	wildcard_lst_from_token(char *token);
bool 		wildcard_matches(const char *name, const char *pattern);

#endif
