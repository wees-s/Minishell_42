/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:41 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:32:42 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "process.h"

static void	update_quotes(char c, int flag[2])
{
	if (c == '\'' && flag[1] == 0)
		flag[0] = !flag[0];
	else if (c == '"' && flag[0] == 0)
		flag[1] = !flag[1];
}

static int	skip_redirection(char *line, int i, int flag[2])
{
	while (line[i] == '>' || line[i] == '<')
		i++;
	while (line[i] == ' ')
		i++;
	while (line[i] && !(line[i] == ' ' && flag[0] == 0 && flag[1] == 0)
		&& line[i] != '<' && line[i] != '>')
	{
		update_quotes(line[i], flag);
		i++;
	}
	while (line[i] == ' ')
		i++;
	return (i);
}

static char	*command_result(char *line, char *res)
{
	int		i;
	int		flag[2];

	i = 0;
	flag[0] = 0;
	flag[1] = 0;
	while (line[i])
	{
		update_quotes(line[i], flag);
		if ((line[i] == '>' || line[i] == '<') && flag[0] == 0 && flag[1] == 0)
		{
			i = skip_redirection(line, i, flag);
			continue ;
		}
		if (line[i] == ' '
			&& flag[0] == 0 && flag[1] == 0
			&& (res[0] == '\0' || res[ft_strlen(res) - 1] == ' '))
		{
			i++;
			continue ;
		}
		res = ft_strjoin_char(res, line[i]);
		i++;
	}
	return (res);
}

char	*command(char *line)
{
	int		len;
	char	*result;

	len = 0;
	result = command_result(line, ft_strdup(""));
	len = ft_strlen(result);
	while (len > 0 && result[len - 1] == ' ')
		result[--len] = '\0';
	if (!result || result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}
