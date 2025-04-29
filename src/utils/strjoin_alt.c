/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin_alt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 00:17:20 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 15:45:19 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Peux etre on devrait free dans tout les cas meme si ca fail*/

char	*ft_strjoin_alt(char *s1, char *s2, t_strjoin free_what)
{
	char		*s1_s2_conc;
	size_t		size_s1_s2_conc;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	size_s1_s2_conc = ft_strlen(s1) + ft_strlen(s2) + 1;
	s1_s2_conc = malloc(sizeof(char) * size_s1_s2_conc);
	if (s1_s2_conc == NULL)
		return (NULL);
	s1_s2_conc[0] = '\0';
	ft_strlcat(s1_s2_conc, s1, size_s1_s2_conc);
	ft_strlcat(s1_s2_conc, s2, size_s1_s2_conc);
	if (free_what == FREE_PARAM1)
		free(s1);
	if (free_what == FREE_PARAM2)
		free(s2);
	if (free_what == (FREE_PARAM1 | FREE_PARAM2))
	{
		free(s1);
		free(s2);
	}
	return (s1_s2_conc);
}
