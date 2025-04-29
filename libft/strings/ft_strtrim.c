/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:03:29 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/29 23:24:52 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ischarstr(const char c, const char *str)
{
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	size;
	size_t	start;
	size_t	end;

	if (!*s1)
		return (ft_calloc(1, sizeof(char)));
	size = ft_strlen(s1) - 1;
	end = size;
	start = 0;
	while (ischarstr(s1[start], set))
		start++;
	while (ischarstr(s1[end], set))
		end--;
	result = ft_substr(s1, start, end - start + 1);
	return (result);
}
// #include <stdio.h>
// int	main(void)
// {
// 	char	*test = "\n\t ";
// 	char	*test2 = ft_strtrim("dsasad", "\n\t ");

// 	printf("%s :", test2);
// }