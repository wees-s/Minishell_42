/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:12 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:31:13 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

static void	valid_quotes_utils(char c, t_valid *v)
{
	if (c == '\'' && !v->quote1)
		v->quote2 = !v->quote2;
	else if (c == '\"' && !v->quote2)
		v->quote1 = !v->quote1;
	v->found_char = 1;
	if (v->last_pipe)
		v->last_pipe = 0;
}

static int	valid_pipe_utils(char *line, t_valid *v, int i, int j)
{
	if ((line[i] == '\'' && !v->quote1) || (line[i] == '\"' && !v->quote2))
		valid_quotes_utils(line[i], v);
	else if (line[i] == '|' && !v->quote1 && !v->quote2)
	{
		while (line[j] && line[j] == ' ')
			j++;
		if (!line[j] || line[j] == '|')
			return (0);
		v->last_pipe = 1;
	}
	else if (line[i] != ' ' && !v->quote1 && !v->quote2)
	{
		v->found_char = 1;
		if (v->last_pipe)
			v->last_pipe = 0;
	}
	if (line[i] != ' ')
		v->found_char = 1;
	return (1);
}

int	valid_pipe(char *line, t_valid *v, int i)
{
	while (line[i] && line[i] == ' ')
		i++;
	if (line[i] == '|')
		return (0);
	if (line[ft_strlen(line) - 1] == '|')
		return (0);
	while (line[i])
	{
		if (valid_pipe_utils(line, v, i, i + 1) == 0)
			return (0);
		i++;
	}
	if (v->last_pipe || !v->found_char)
		return (0);
	return (1);
}

static int	valid_red_utils(char *line, char c, int *i)
{
	int	count;
	int	j;

	count = 0;
	while (line[*i + count] == c)
		count++;
	if (count > 2)
		return (0);
	if ((c == '>' && line[*i + count] == '<')
		|| (c == '<' && line[*i + count] == '>'))
		return (0);
	j = *i + count;
	while (line[j] && line[j] == ' ')
		j++;
	if (!line[j] || line[j] == '|' || line[j] == '>' || line[j] == '<')
		return (0);
	*i += count - 1;
	return (1);
}

int	valid_red(char *line, char c)
{
	int	i;
	int	quote1;
	int	quote2;

	i = 0;
	quote1 = 0;
	quote2 = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !quote1)
			quote2 = !quote2;
		else if (line[i] == '\"' && !quote2)
			quote1 = !quote1;
		else if (line[i] == c && !quote1 && !quote2)
		{
			if (!valid_red_utils(line, c, &i))
				return (0);
		}
		i++;
	}
	return (1);
}
