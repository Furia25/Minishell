/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin_alt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 00:17:20 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/30 16:41:11 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_params(char *s1, char *s2, enum e_strjoin free_what)
{
	if (free_what == FREE_PARAM1)
		free(s1);
	if (free_what == FREE_PARAM2)
		free(s2);
	if (free_what == (FREE_PARAM1 | FREE_PARAM2))
	{
		free(s1);
		free(s2);
	}
}

char	*ft_strjoin_alt(char *s1, char *s2, t_strjoin free_what)
{
	char		*s1_s2_conc;
	size_t		size_s1_s2_conc;

	if (s1 == NULL || s2 == NULL)
	{
		free_params(s1, s2, free_what);
		return (NULL);
	}
	size_s1_s2_conc = ft_strlen(s1) + ft_strlen(s2) + 1;
	s1_s2_conc = malloc(sizeof(char) * size_s1_s2_conc);
	if (s1_s2_conc == NULL)
	{
		free_params(s1, s2, free_what);
		return (NULL);
	}
	s1_s2_conc[0] = '\0';
	ft_strlcat(s1_s2_conc, s1, size_s1_s2_conc);
	ft_strlcat(s1_s2_conc, s2, size_s1_s2_conc);
	free_params(s1, s2, free_what);
	return (s1_s2_conc);
}

void	free_params_gc(char *s1, char *s2, enum e_strjoin free_what, t_minishell *data)
{
	if (free_what == FREE_PARAM1)
		gc_free(s1, data);
	if (free_what == FREE_PARAM2)
		gc_free(s2, data);
	if (free_what == (FREE_PARAM1 | FREE_PARAM2))
	{
		gc_free(s1, data);
		gc_free(s2, data);
	}
}

char	*ft_strjoin_alt_gc(char *s1, char *s2, t_strjoin free_what, t_minishell *data)
{
	char		*s1_s2_conc;
	size_t		size_s1_s2_conc;

	if (s1 == NULL || s2 == NULL)
	{
		free_params_gc(s1, s2, free_what, data);
		return (NULL);
	}
	size_s1_s2_conc = ft_strlen(s1) + ft_strlen(s2) + 1;
	s1_s2_conc = malloc(sizeof(char) * size_s1_s2_conc);
	if (s1_s2_conc == NULL)
	{
		free_params_gc(s1, s2, free_what, data);
		return (NULL);
	}
	s1_s2_conc[0] = '\0';
	ft_strlcat(s1_s2_conc, s1, size_s1_s2_conc);
	ft_strlcat(s1_s2_conc, s2, size_s1_s2_conc);
	free_params_gc(s1, s2, free_what, data);
	return (s1_s2_conc);
}
