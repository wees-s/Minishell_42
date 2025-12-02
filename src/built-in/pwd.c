/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:23 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/28 14:39:50 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

t_env	*get_value_by_name(t_env **new_env, char *str)
{
	t_env	*list_reset;
	t_env	*value;

	list_reset = *new_env;
	while (*new_env)
	{
		if (ft_strcmp((*new_env)->key, str) == 0)
		{
			value = (*new_env);
			(*new_env) = list_reset;
			return (value);
		}
		(*new_env) = (*new_env)->next;
	}
	(*new_env) = list_reset;
	return (NULL);
}

void	pwd(t_shell *sh)
{
	t_env	*pointer;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		sh->last_exit_status = 1;
		return (perror("pwd"));
	}
	printf("%s\n", pwd);
	pointer = get_value_by_name(&sh->env, "PWD");
	if (pointer)
		update_value(pwd, pointer);
	sh->last_exit_status = 0;
	free(pwd);
}
