/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:35 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 13:44:07 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

static int	check_flag(char *token)
{
	int	i;

	i = 0;
	if (!token)
		return (0);
	if (token[i] == '-' && token[i + 1] == 'n')
	{
		i++;
		while (token[i] != '\0')
		{
			if (token[i] != 'n')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

static void	result_exit(t_shell *sh)
{
	if (!g_heredoc_child)
		printf("%d\n", sh->last_exit_status);
	else
		printf("%d\n", g_heredoc_child + 128);
	sh->last_exit_status = 0;
	g_heredoc_child = 0;
}

static char	*result_echo(char **token, t_shell *sh, int flag, int i)
{
	char	*result;
	char	*temp;

	temp = NULL;
	result = NULL;
	if (ft_strncmp(token[1], "$?", 3) == 0)
		result_exit(sh);
	else if ((flag && token[2] != NULL) || flag)
	{
		result = ft_join_all(token, i);
		sh->last_exit_status = 0;
		temp = remove_quotes_str(result, 0, 0);
		printf("%s", temp);
	}
	else if (!flag)
	{
		result = ft_join_all(token, i);
		sh->last_exit_status = 0;
		temp = remove_quotes_str(result, 0, 0);
		printf("%s\n", temp);
	}
	free(temp);
	return (result);
}

void	echo(char **token, t_shell *sh)
{
	int		i;
	int		flag;
	char	*result;

	i = 1;
	flag = 0;
	if (token[1] == NULL)
	{
		printf("\n");
		sh->last_exit_status = 0;
		return ;
	}
	if (check_flag(token[i]))
	{
		flag = 1;
		while (check_flag(token[i]))
			i++;
	}
	result = result_echo(token, sh, flag, i);
	if (result != NULL)
		free(result);
}
