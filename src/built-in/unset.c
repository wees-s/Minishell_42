/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:19 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/28 14:39:32 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

static void	remove_arg(t_shell *sh, char **cmd, int i)
{
	t_env	*curr;
	t_env	*prev;

	curr = sh->env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, cmd[i], ft_strlen(cmd[i]) + 1) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				sh->env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	unset_env(t_shell *sh, char **cmd)
{
	int	i;

	i = 0;
	sh->last_exit_status = 0;
	if (cmd[1] == NULL)
		return ;
	while (cmd[i])
	{
		remove_arg(sh, cmd, i);
		i++;
	}
}
