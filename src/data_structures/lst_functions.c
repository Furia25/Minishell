/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:14:01 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/07 22:25:25 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*lstnew(char *lexeme)
{
	t_lst	*new;

	new = malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->lexeme = lexeme;
	new->next = NULL;
	return (new);
}

t_lst	*lstlast(t_lst *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	lstadd_back(t_lst **lst, t_lst *new)
{
	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	lstlast(*lst)->next = new;
}

void	lstdelone(t_lst *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	if (del == NULL)
	{
		free(lst);
		return ;
	}
	del(lst->lexeme);
	free(lst);
}

void	lstclear(t_lst **lst, void (*del)(void*))
{
	t_lst	*next_alt;

	if (lst == NULL)
		return ;
	while (*lst != NULL)
	{
		next_alt = (*lst)->next;
		lstdelone(*lst, del);
		*lst = next_alt;
	}
}

