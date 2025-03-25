/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:59:32 by alpayet           #+#    #+#             */
/*   Updated: 2025/02/25 00:51:28 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*s_copy;
	size_t	s_len;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		s_copy = malloc(1);
		if (s_copy == NULL)
			return (NULL);
		s_copy[0] = '\0';
		return (s_copy);
	}
	if (len > s_len - start)
		len = s_len - start;
	if (len == SIZE_MAX)
		return (NULL);
	s_copy = malloc((len + 1) * sizeof(char));
	if (s_copy == NULL)
		return (NULL);
	ft_strlcpy(s_copy, s + start, len + 1);
	return (s_copy);
}

/*int	main(void)
{
	char str[]= "Hola";
	printf("%s\n", ft_substr(str, 0, 3));

	printf("%s\n", str);
}*/