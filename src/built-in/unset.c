/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:19 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:33:20 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

void	unset_env(t_shell *sh, char **cmd)
{
	t_env	*curr;
	t_env	*prev;

	sh->last_exit_status = 0;
	if (cmd[1] == NULL)
		return ;
	curr = sh->env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, cmd[1], ft_strlen(cmd[1]) + 1) == 0)
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
