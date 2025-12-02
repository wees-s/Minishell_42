/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:26 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/28 16:21:38 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

static void	exit_args(char **tokens, t_shell *sh, int i, int is_num)
{
	char	*token;
	int		status;

	token = remove_quotes_str(tokens[1], 0, 0);
	while (token[i])
	{
		if (!isdigit(token[i]) && !((token[0] == '+' || token[0] == '-')))
			is_num = -1;
		i++;
	}
	if (is_num == -1)
	{
		printf("exit: %s: numeric argument required\n", tokens[1]);
		sh->last_exit_status = 2;
	}
	else
	{
		status = ft_atoi(token);
		status %= 256;
		if (status < 0)
			status += 256;
		sh->last_exit_status = status;
	}
	free(token);
}

void	exit_process(char **tokens, t_shell *sh)
{
	if (tokens[1] && tokens[2])
	{
		printf("exit: too many arguments\n");
		sh->last_exit_status = 1;
		return ;
	}
	if (tokens[1])
		exit_args(tokens, sh, 0, 0);
	else
		sh->last_exit_status = 0;
	if (sh->s_pipe)
		free_array(sh->s_pipe);
	close(sh->fd_in);
	close(sh->fd_out);
	free_list(&sh->env);
	free_array(tokens);
	rl_clear_history();
	exit(sh->last_exit_status);
}
