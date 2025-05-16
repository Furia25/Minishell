/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 04:06:07 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 04:12:29 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H
# include "minishell.h"


void	handle_red_input(t_leaf *cmd, char *file, t_minishell *data);
void	handle_red_output(t_leaf *cmd, char *file, t_minishell *data);
void	handle_red_output_append(t_leaf *cmd, char *file, t_minishell *data);
void	handle_red_input_output(t_leaf *cmd, char *file, t_minishell *data);
void	redirections_in_par_cmd(t_leaf *cmd, t_minishell *data);
void	redirections_cmd(t_leaf *cmd, t_minishell *data);
void	close_all_fds(t_leaf *command_tab);

#endif
