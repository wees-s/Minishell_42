/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:04 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 12:46:38 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

void	exec_access_perror(char *s, char **array, int x, t_env *env)
{
	perror(s);
	free_list(&env);
	free_array(array);
	exit(x);
}

void	exec_access_putstr(char *s, char **array, int x, t_env *env)
{
	ft_putstr_fd(s, 2);
	free_list(&env);
	free_array(array);
	exit(x);
}

char	**path(t_env *env)
{
	char	*value_raw;
	char	*value_path;
	char	**path_split;

	value_raw = expanded(env, "PATH");
	if (value_raw == NULL)
		return (NULL);
	value_path = ft_strdup(value_raw);
	if (value_path == NULL)
		return (NULL);
	path_split = ft_split(value_path, ':');
	free(value_path);
	return (path_split);
}

char	*command_valid(char **tokens, char **path_split)
{
	int		i;
	char	*temp_path;
	char	*exec;

	i = 0;
	while (path_split[i])
	{
		temp_path = ft_strjoin(path_split[i], "/");
		exec = ft_strjoin(temp_path, tokens[0]);
		free(temp_path);
		if (access(exec, F_OK | X_OK) == 0)
			return (exec);
		else if (access(exec, F_OK) == 0 && access(exec, X_OK) != 0)
		{
			free(exec);
			return ((char *)-1);
		}
		free(exec);
		i++;
	}
	return (NULL);
}
