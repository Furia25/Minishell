/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:36:49 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/14 11:05:40 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H
# include "minishell.h"

void			*gc_calloc(size_t count, size_t size, t_minishell *data);
void			*gc_malloc(size_t size, t_minishell *data);
void			gc_free(void *pointer, t_minishell *data);
void			gc_free_node(t_lst *node, t_minishell *data);
void			gc_free_ast(t_AST_node	*ast_node, t_minishell *data);
void			gc_add(void *ptr, t_minishell *data);
void			gc_clean(t_minishell *data);
bool			gc_init(t_minishell *data);

#endif
