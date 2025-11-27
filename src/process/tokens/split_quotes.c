/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:49 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:32:50 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

static void	handle_quote_state(char c, int *in_quotes, char *quote_char)
{
	if (c == '\'' || c == '"')
	{
		if (!*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = c;
		}
		else if (c == *quote_char)
			*in_quotes = 0;
	}
}

static int	count_args(char *line)
{
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	in_quotes = 0;
	while (*line)
	{
		while (*line == ' ' && !in_quotes)
			line++;
		if (!*line)
			break ;
		count++;
		while (*line && (in_quotes || *line != ' '))
		{
			handle_quote_state(*line, &in_quotes, &quote_char);
			line++;
		}
	}
	return (count);
}

static int	find_arg_end(char *line, int start)
{
	int		i;
	int		in_quotes;
	char	quote_char;

	i = start;
	in_quotes = 0;
	while (line[i] && (in_quotes || line[i] != ' '))
	{
		handle_quote_state(line[i], &in_quotes, &quote_char);
		i++;
	}
	return (i);
}

char	**split_with_quotes(char *line)
{
	int		start;
	int		end;
	int		i;
	int		count;
	char	**res;

	count = count_args(line);
	res = malloc((count + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	start = 0;
	i = 0;
	while (line[start])
	{
		while (line[start] == ' ')
			start++;
		if (!line[start])
			break ;
		end = find_arg_end(line, start);
		res[i] = ft_strndup(line + start, end - start + 1);
		i++;
		start = end;
	}
	res[i] = NULL;
	return (res);
}
