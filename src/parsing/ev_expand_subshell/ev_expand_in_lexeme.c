/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_in_lexeme.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:47:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/17 16:31:40 by alpayet          ###   ########.fr       */
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
	while (str[i] == '_' || ft_isalnum(str[i]) == true)
		i++;
	return (i);
}

char	*ev_result(char *str, size_t ev_len, t_minishell *data)
{
	char			*dup;
	t_hash_entry	*entry;
	t_envvar		*var;

	if (ev_len == 0)
		return (check_malloc(ft_calloc(1, sizeof(char)), data));
	if (str[0] == '?')
		return (check_malloc(ft_itoa(data->exit_code), data));
	entry = hashmap_search(hash(str), &data->environment);
	if (entry == NULL)
		return (check_malloc(ft_calloc(1, sizeof(char)), data));
	var = (t_envvar *)entry->value;
	dup = ft_strdup(var->value);
	check_malloc(dup, data);
	return (dup);
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
		&& str[i + 1] != '?' && str[i + 1] != '_' && str[i + 1] != '\0')
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
	return (check_malloc(ft_strdup(str), data));
}
