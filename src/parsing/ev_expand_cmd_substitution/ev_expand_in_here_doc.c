/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_in_here_doc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:54:11 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/27 17:54:25 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t		skip_cmd_sub_and_special_chara(char *str);
size_t		env_var_len(char *str);
char		*ev_result(char *str, size_t ev_len, t_minishell *data);
char		*handle_all_ev_expand_in_here_doc(char *str, t_minishell *data);

static char	*ev_expand_in_here_doc(char *str, char *ev_str, t_minishell *data)
{
	size_t	ev_len;
	char	*until_ev;
	char	*after_ev;
	char	*final_result;

	ev_len = env_var_len(ev_str);
	until_ev = ft_strjoin_alt_gc(str, ev_result(ev_str, ev_len, data),
			FREE_PARAM2, data);
	check_malloc(until_ev, data);
	after_ev = handle_all_ev_expand_in_here_doc(ev_str + ev_len, data);
	final_result = ft_strjoin_alt_gc(until_ev, after_ev,
			FREE_PARAM1 | FREE_PARAM2, data);
	check_malloc(final_result, data);
	return (final_result);
}

char	*handle_all_ev_expand_in_here_doc(char *str, t_minishell *data)
{
	size_t	i;
	char	*str_dup;

	i = 0;
	while (str[i])
	{
		i += skip_cmd_sub_and_special_chara(str + i);
		if (str[i] == '$' && str[i + 1] != '\0'
			&& str[i + 1] != '\'' && str[i + 1] != '\"')
		{
			str[i] = '\0';
			i++;
			return (ev_expand_in_here_doc(str, str + i, data));
		}
		i++;
	}
	str_dup = ft_strdup(str);
	check_malloc(str_dup, data);
	return (str_dup);
}
