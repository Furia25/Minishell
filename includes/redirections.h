/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 04:06:07 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/23 15:01:57 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H
# include "minishell.h"

# define HERE_DOC_FILE "/tmp/here_doc"

void	handle_red_input(t_leaf *cmd, t_lexeme_type token_type, char *file);
void	handle_red_output(t_leaf *cmd, char *file);
void	handle_red_output_append(t_leaf *cmd, char *file);
void	handle_red_input_output(t_leaf *cmd, char *file);
bool	handle_all_here_doc(t_leaf *command_tab, t_minishell *data);
void	redirections_in_cmd(t_leaf *cmd, t_minishell *data);
void	redirections_in_par_cmd(t_leaf *cmd, t_minishell *data);
void	rm_all_here_doc_files(t_leaf *command_tab);
void	close_all_fds(t_leaf *command_tab);

#endif
