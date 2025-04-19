// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   subshell_double_quotes.c                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/18 20:39:04 by alpayet           #+#    #+#             */
// /*   Updated: 2025/04/18 23:34:12 by alpayet          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"
// void	not_interpret_chara(char chara, char *str);

// char	*stock_file_in_str(int fd)
// {
// 	char	*str;
// 	char	*buff;

// 	str = malloc(1);
// 	str[0] = '\0';
// 	buff = get_next_line(fd);
// 	while (buff)
// 	{
// 		str = ft_strjoin_alt(str, buff, FREE_PARAM1 | FREE_PARAM2);
// 		buff = get_next_line(fd);
// 	}
// 	return (str);
// }

// char	*subshell_in_DQ(char *str, size_t *i_ptr)
// {
// 	size_t	j;
// 	int		pipefd[2];
// 	int		pipefd2[2];
// 	pid_t		pid;

// 	j = 0;
// 	while (str[j] != '\0' && str[j] != ')')
// 		j++;
// 	if (str[j] == '\0')
// 		not_interpret_chara('(', "\' (unclosed parenthesis)");
// 	*i_ptr = *i_ptr + j + 1;
// 	pipe(pipefd);
// 	pipe(pipefd2);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		close(pipefd[1]);
// 		close(pipefd2[0]);
// 		dup2(pipefd[0], 0);
// 		close(pipefd[0]);
// 		dup2(pipefd2[1], 1);
// 		close(pipefd2[1]);
// 		ft_printf("%s", get_next_line(0));
// 		exit(0);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd2[1]);
// 	write(pipefd[1], "str", 3);
// 	close(pipefd[1]);
// 	wait(NULL);
// 	str = stock_file_in_str(pipefd2[0]);
// 	close(pipefd2[0]);
// 	return (str);
// }

// char	*handle_dollars_in_DQ(char *str)
// {
// 	size_t	i;
// 	char	*buff;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			str[i] = '\0';
// 			i++;
// 			if (str[i] == '(')
// 			{
// 				buff = ft_strjoin_alt(str, subshell_in_DQ(str + i, &i), FREE_PARAM2);
// 				return (ft_strjoin_alt(buff, handle_dollars_in_DQ(str + i), FREE_PARAM1 | FREE_PARAM2));
// 			}
// 		}
// 		i++;
// 	}
// 	return (ft_substr(str, 0, ft_strlen(str)));
// }

// // int	main(void)
// // {
// // 	char	*str;

// // 	str = "dsa$(ls)fd";
// // 	str = ft_substr(str, 0, ft_strlen(str));
// // 	ft_printf("%s\n", handle_dollars_in_DQ(str));
// // }