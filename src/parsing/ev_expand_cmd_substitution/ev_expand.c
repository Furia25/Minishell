/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:21:50 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/02 14:15:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"

char	*handle_all_ev_expand(char *str, t_lexeme_type next_type,
			t_minishell *data);
void	clean_wildcards_symbol(char *str);

size_t	env_var_len(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '?' || str[i] == - '?')
		return (1);
	while (str[i] == '_' || ft_isalnum(str[i]) == true)
		i++;
	return (i);
}

char	*ev_result(char *str, size_t ev_len, t_minishell *data)
{
	char			*str_key;
	char			*dup;
	t_hash_entry	*entry;
	t_envvar		*var;

	if (ev_len == 0)
		return (check_malloc(ft_calloc(1, sizeof(char)), data));
	if (str[0] == '?' || str[0] == - '?')
		return (check_malloc(ft_itoa(data->exit_code), data));
	str_key = ft_substr(str, 0, ev_len);
	check_malloc(str_key, data);
	entry = hashmap_search(hash(str_key), &data->environment);
	if (entry == NULL)
		return (check_malloc(ft_calloc(1, sizeof(char)), data));
	gc_free(str_key, data);
	var = (t_envvar *)entry->value;
	dup = ft_strdup(var->value);
	check_malloc(dup, data);
	return (dup);
}

size_t	skip_cmd_sub_and_special_chara(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '$' && str[i + 1] == '(')
		i = i + 1 + in_parentheses_len(str + i + 1) + 1;
	if (str[i] == '$' && ft_isalnum(str[i + 1]) == false
		&& ft_strchr(HANDLE_EV_EXPAND_CASES, str[i + 1]) == NULL)
		i++;
	return (i);
}

char	*ev_expand(char *str, char *ev_str, t_lexeme_type next_type,
		t_minishell *data)
{
	size_t	ev_len;
	char	*until_ev;
	char	*after_ev;
	char	*final_result;

	ev_len = env_var_len(ev_str);
	until_ev = ft_strjoin_alt_gc(str, ev_result(ev_str, ev_len, data),
			FREE_PARAM2, data);
	check_malloc(until_ev, data);
	after_ev = handle_all_ev_expand(ev_str + ev_len, next_type, data);
	final_result = ft_strjoin_alt_gc(until_ev, after_ev,
			FREE_PARAM1 | FREE_PARAM2, data);
	check_malloc(final_result, data);
	return (final_result);
}

char	*handle_all_ev_expand(char *str, t_lexeme_type next_type,
		t_minishell *data)
{
	size_t	i;
	char	*str_dup;

	i = 0;
	while (str[i])
	{
		i += skip_cmd_sub_and_special_chara(str + i);
		if (str[i] == '$' && str[i + 1] != '\'')
		{
			if ((str[i + 1] != '\0' || (next_type == DOUBLE_Q
						|| next_type == SINGLE_Q)))
			{
				str[i] = '\0';
				i++;
				return (ev_expand(str, str + i, next_type, data));
			}
		}
		i++;
	}
	str_dup = ft_strdup(str);
	check_malloc(str_dup, data);
	return (str_dup);
}
