/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:52 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:33:53 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

static void	put_exp(char **temp, t_env **new_env)
{
	char	*value;
	char	*tmp;
	char	*result;

	if (temp[1] == NULL)
	{
		put_env(new_env, temp[0]);
		return ;
	}
	value = remove_quotes_str(temp[1], 0, 0);
	tmp = ft_strjoin(temp[0], "=");
	result = ft_strjoin(tmp, value);
	free(value);
	free(tmp);
	put_env(new_env, result);
	free(result);
}

void	update_value(char *split_line, t_env *pointer)
{
	char	*result;

	result = remove_quotes_str(split_line, 0, 0);
	free(pointer->value);
	pointer->value = result;
}

void	check_to_put(char *split_line, t_env **env)
{
	int		flag;
	char	**temp;
	t_env	*current;

	flag = 0;
	temp = split_env(split_line);
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, temp[0]) == 0)
		{
			update_value(temp[1], current);
			flag = 1;
			break ;
		}
		current = current->next;
	}
	if (!flag)
		put_exp(temp, env);
	free_array(temp);
}
