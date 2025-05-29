/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:03:29 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/29 17:12:40 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(const char *s1, const char *set)
{
	const char	*start;
	const char	*end;
	char		*out;
	size_t		len;

	if (!s1 || !set)
		return (ft_calloc(1, 1));
	start = s1;
	while (*start && ft_strchr(set, *start))
		start++;
	end = start + ft_strlen(start);
	while (end > start && ft_strchr(set, *(end - 1)))
		end--;
	len = end - start;
	out = ft_calloc(len + 1, 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, start, len);
	return (out);
}
