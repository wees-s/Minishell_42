/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:26 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:31:27 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utils/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_list(t_env **begin_list)
{
	t_env	*to_free;

	to_free = NULL;
	if (!begin_list || !*begin_list)
		return ;
	while (*begin_list)
	{
		free(((*begin_list)->key));
		free(((*begin_list)->value));
		to_free = (*begin_list);
		(*begin_list) = (*begin_list)->next;
		free(to_free);
	}
	*begin_list = NULL;
}
