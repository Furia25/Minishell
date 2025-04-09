/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:49:08 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/09 02:03:50 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "wildcards.h"
#include "libft.h"
#include "stdlib.h"
#include <stdint.h>

t_wsearch	wildcard_lst_from_token(char *token)
{
	t_wildcard	wildcard;
	t_wsearch	search_result;
	size_t		index;

	wildcard.component = ft_split(token, PATH_SYMBOL);
	if (!wildcard.component)
		return ((t_wsearch) {-1, NULL});
	index = 0;
	wildcard.count = 0;
	while (wildcard.component[index] != NULL)
	{
		wildcard.count += 1;
		index++;
	}
	wildcard.result = NULL;
	wildcard.token = token;
	search_result = wildcard_explore(".", &wildcard, 0);
	free_chartab(wildcard.component);
	return (search_result);
}

bool wildcard_matches(const char *name, const char *pattern)
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
			return (false);
	}
	while (*pattern_ptr == '*')
		pattern_ptr++;
	return *pattern_ptr == '\0';
}
