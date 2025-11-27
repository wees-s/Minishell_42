/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:38 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:33:39 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

static void	update_pwd(t_env **begin_list)
{
	char	*temp;
	t_env	*node;

	temp = NULL;
	node = *begin_list;
	while (node)
	{
		if (ft_strcmp(node->key, "PWD") == 0)
		{
			temp = getcwd(NULL, 0);
			free(node->value);
			node->value = temp;
		}
		node = node->next;
	}
}

static void	update_oldpwd(t_env **begin_list)
{
	t_env	*node;
	char	*temp;

	node = *begin_list;
	while (node)
	{
		if (ft_strcmp(node->key, "OLDPWD") == 0)
		{
			if (ft_strcmp(node->key, "PWD") == 0)
				temp = ft_strdup(expanded(*begin_list, "PWD"));
			else if (ft_strcmp(node->key, "PWD") != 0)
				temp = getcwd(NULL, 0);
			free(node->value);
			node->value = temp;
		}
		node = node->next;
	}
}

static void	cd_utils(char **line, t_shell *sh)
{
	int	flag;

	flag = chdir(line[1]);
	if (flag == -1)
	{
		printf("cd: %s: No such file or directory\n", line[1]);
		sh->last_exit_status = 1;
	}
	else
	{
		update_oldpwd(&sh->env);
		update_pwd(&sh->env);
	}
}

void	cd(char **line, t_shell *sh)
{
	sh->last_exit_status = 0;
	if (line[0] && line[1] && line[2])
	{
		printf("cd: too many arguments\n");
		sh->last_exit_status = 1;
		return ;
	}
	if (!line[1] || line[1][0] == '~')
	{
		if (!expanded(sh->env, "HOME"))
		{
			printf("cd: HOME not set\n");
			sh->last_exit_status = 1;
			return ;
		}
		chdir(expanded(sh->env, "HOME"));
		update_pwd(&sh->env);
	}
	else
		cd_utils(line, sh);
}
