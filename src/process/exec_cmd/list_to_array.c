/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:54 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:32:55 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

static int	list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_list_to_array(t_env *env, int i, char *tmp1, char *tmp2)
{
	int		size;
	char	**array;

	size = list_size(env);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	while (env)
	{
		tmp1 = ft_strjoin(env->key, "=");
		if (env->value)
		{
			tmp2 = ft_strjoin(tmp1, env->value);
			free(tmp1);
			array[i] = tmp2;
		}
		else
			array[i] = tmp1;
		i++;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
