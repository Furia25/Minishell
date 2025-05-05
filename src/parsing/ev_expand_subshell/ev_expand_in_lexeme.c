/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_in_lexeme.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:47:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/05 17:30:07 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*handle_ev_in_lexeme(char *str, t_lexeme_type next_type, t_minishell *data);

size_t	env_var_len(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '?')
		return (1);	
	while (ft_isalnum(str[i]) == true)
		i++;
	return (i);
}

char	*ev_result(char *str, size_t ev_len, t_minishell *data)
{
	//retirer les void
	(void)str;
	(void)data;
	if (ev_len == 0)
		return (ft_calloc(1, sizeof(char)));
	return (ft_substr("a b", 0, 3));
}

size_t	skip_subshell_and_special_chara(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '$' && str[i + 1] == '(')
	{
		while (str[i] != ')')
			i++;
	}
	if (str[i] == '$' && ft_isalnum(str[i + 1]) == false
		&& str[i + 1] != '?' && str[i + 1] != '\0')
		i++;
	return (i);
}

static char	*ev_expand(char *str, char *ev_str, t_lexeme_type next_type, t_minishell *data)
{
	size_t	ev_len;
	char	*buff;

	ev_len = env_var_len(ev_str);
	buff = ft_strjoin_alt_gc(str, ev_result(ev_str, ev_len, data),
	FREE_PARAM2, data);
	check_malloc(buff, data);
	return (check_malloc(ft_strjoin_alt_gc(buff,
		handle_ev_in_lexeme(ev_str + ev_len, next_type, data),
		FREE_PARAM1 | FREE_PARAM2, data), data));
}

char	*handle_ev_in_lexeme(char *str, t_lexeme_type next_type, t_minishell *data)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i += skip_subshell_and_special_chara(str + i);
		if (str[i] == '$' && str[i + 1] != '\'')
		{
			if ((str[i + 1] != '\0'
				|| (next_type == DOUBLE_Q || next_type == SINGLE_Q)))
			{
				str[i] = '\0';
				i++;
				return (ev_expand(str, str + i, next_type, data));
			}
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
}
