/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:59 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 12:35:30 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

void	cmd_bar(char **tokens, t_env *env, int fd_in, int fd_out)
{
	char	**envp;

	if (access(tokens[0], F_OK) != 0)
	{
		dup2_close_in_out(fd_in, fd_out);
		exec_access_putstr("No such file or directory\n", tokens, 127, env);
	}
	if (access(tokens[0], X_OK) != 0)
	{
		dup2_close_in_out(fd_in, fd_out);
		exec_access_putstr("Permission denied\n", tokens, 126, env);
	}
	envp = env_list_to_array(env, 0, ft_strdup(""), ft_strdup(""));
	execve(tokens[0], tokens, envp);
	free_array(envp);
	dup2_close_in_out(fd_in, fd_out);
	exec_access_perror("Error execve", tokens, 126, env);
}

static void	free_all(char **path_split, char *exec, char **envp)
{
	free_array(path_split);
	free(exec);
	free_array(envp);
}

static char	**path_split_func(char **tokens, t_shell *sh)
{
	char	**path_split;

	path_split = path(sh->env);
	if (path_split == NULL)
	{
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		exec_access_putstr("No such file or directory\n", tokens, 121, sh->env);
		return (NULL);
	}
	else
		return (path_split);
}

void	cmd_not_bar(char **tokens, t_shell *sh)
{
	char	**path_split;
	char	*exec;
	char	**envp;

	path_split = path_split_func(tokens, sh);
	if (path_split == NULL)
		return ;
	exec = command_valid(tokens, path_split);
	if (exec == (char *)-1)
	{
		free_array(path_split);
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		exec_access_putstr("Permission denied\n", tokens, 126, sh->env);
	}
	else if (!exec)
	{
		free_array(path_split);
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		exec_access_putstr("Command not found\n", tokens, 127, sh->env);
	}
	envp = env_list_to_array(sh->env, 0, ft_strdup(""), ft_strdup(""));
	execve(exec, tokens, envp);
	free_all(path_split, exec, envp);
	dup2_close_in_out(sh->fd_in, sh->fd_out);
	exec_access_perror("Error execve", tokens, 126, sh->env);
}
